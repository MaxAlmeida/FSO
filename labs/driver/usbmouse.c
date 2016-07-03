#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/hid.h>
#define BUF_LEN 18
#include <linux/usb/input.h>

#define VENDOR_ID   0x046d
#define PRODUCT_ID  0xc534

/* Global variables */
static unsigned int device_open = 0;  /* Prevent multiples access */
static struct usb_device *device;     /* The USB device itself */
static char msg[BUF_LEN];             /* The msg the device will give */
static char *msg_ptr;
static int counter = 0;

/* Define witch device are work with */
static struct usb_device_id mouse_table[]={
  {USB_DEVICE(VENDOR_ID, PRODUCT_ID)},
  {} /* Terminating Entry */
};
MODULE_DEVICE_TABLE(usb,mouse_table);

struct usb_mouse{
  char name[128];
  char phys[64];
  struct input_dev *dev;
  struct urb *irq;
  struct usb_device *usbdev;
  signed char *data;
  dma_addr_t data_dma;


};

static void usb_mouse_irq(struct urb *urb)
{
          struct usb_mouse *mouse = urb->context;
          signed char *data = mouse->data;
          struct input_dev *dev = mouse->dev;
          int status;
  
          switch (urb->status) {
          case 0:                 /* success */
                  break;
          case -ECONNRESET:       /* unlink */
          case -ENOENT:
          case -ESHUTDOWN:
                  return;
          /* -EPIPE:  should clear the halt */
          default:                /* error */
                  goto resubmit;
          }
  
          input_report_key(dev, BTN_LEFT,   data[0] & 0x01);
          input_report_key(dev, BTN_RIGHT,  data[0] & 0x02);
          input_report_key(dev, BTN_MIDDLE, data[0] & 0x04);
          input_report_key(dev, BTN_SIDE,   data[0] & 0x08);
          input_report_key(dev, BTN_EXTRA,  data[0] & 0x10);
  
          input_report_rel(dev, REL_X,     data[1]);
          input_report_rel(dev, REL_Y,     data[2]);
          input_report_rel(dev, REL_WHEEL, data[3]);
  
          input_sync(dev);
  resubmit:
          status = usb_submit_urb (urb, GFP_ATOMIC);
          if (status)
                  dev_err(&device->dev,
                          "can't resubmit intr, %s-%s/input0, status %d\n",
                          device->bus->bus_name,
                          device->devpath, status);
  }
/* Called when a process tries to open the device file
 * like "cat /dev/FILE" */
static int mouse_open(struct input_dev *dev){
  struct usb_mouse *mouse = input_get_drvdata(dev);  
  if(device_open){
    printk(KERN_ERR "[%d]Busy!\n", device_open);
    return -EBUSY;
  }

  device_open++;
  sprintf(msg, "[%d]Mouse opened\n", counter++);
  msg_ptr = msg;
  
  mouse->irq->dev = mouse->usbdev;
  if(usb_submit_urb(mouse->irq, GFP_KERNEL))
     return -EIO;
  return 0; /* Return Success */
}

/* Called when a process closes the device file */
static int mouse_close( struct input_dev *dev){
  printk(KERN_INFO "[%d]Mouse closed\n", counter);
  device_open--; /* Release the file to the next call */

  struct usb_mouse *mouse = input_get_drvdata(dev);
  usb_kill_urb(mouse->irq);

  return 0;
}

/* Called when a process attempts to read */
static ssize_t mouse_read(
    struct file *file,  /* File pointer */
    char *buffer,       /* Buffer to fill with data */
    size_t length,      /* Length of the buffer*/
    loff_t *offset){    /* Offset int the file */

  int bytes_read = 0; /* Number of bytes written to the buffer */
  if(*msg_ptr == 0){ /* If it's the end of the file */
    return 0;
  }

  /* Putting the data into the buffer */
  while(length && *msg_ptr){
    /* The buffer is in the user data segment
     * We have to use put_user which copies data from
     *  the kernel data segment to the user data segment */
    put_user(*(msg_ptr++), buffer++);
    length--;
    bytes_read++;
  }

  printk(KERN_INFO "mouse: Reading bytes [%d]\n", bytes_read);
  return bytes_read;
}

/* Called when a process writes to dev file
 * like "hello > /dev/FILE"*/
static ssize_t mouse_write(
    struct file *file,
    const char *buff,
    size_t length,
    loff_t *off){

  printk(KERN_INFO "Operation not supported!\n");
  return -EINVAL;
}

/* File operation structs */
static struct file_operations fops = {
  open:     mouse_open,
  release:  mouse_close,
  read:     mouse_read,
  write:    mouse_write,
};

static struct usb_class_driver usb_class = {
  name: "usb/mouse%d",
  fops: &fops,
};

/* Called on device insertion. Only if no other driver has loaded */
static int mouse_probe(
    struct usb_interface *interface,
    const struct usb_device_id *id){
  struct usb_endpoint_descriptor *endpoint;
  printk(KERN_INFO "mouse: mouse_probe: starting\n");
  device = interface_to_usbdev(interface);
  struct usb_mouse *mouse;
  struct input_dev *input_dev;
  int pipe, maxp;
  
  pipe = usb_rcvintpipe(device, endpoint->bEndpointAddress);
  maxp = usb_maxpacket(device, pipe, usb_pipeout(pipe));
  int retval = usb_register_dev(interface, &usb_class);
  /* Try to register the device */
  if(retval < 0){
    printk(KERN_ERR "mouse: Not able to get a minor for this device. Error %d", retval);
    return retval;
  }else{
    printk(KERN_INFO "mouse: Minor obtained: %d\n", interface->minor);
  }
 
  mouse = kzalloc(sizeof(struct usb_mouse), GFP_KERNEL);
  input_dev = input_allocate_device();
  //fail1
  
  mouse-> data = usb_alloc_coherent(device, 8, GFP_ATOMIC, &mouse->data_dma);
  //fail1

  mouse->irq = usb_alloc_urb(0, GFP_KERNEL);
  
  mouse-> usbdev = device;
  mouse->dev = input_dev;
  
  usb_make_path(device, mouse->phys, sizeof(mouse->phys));
  strlcat(mouse->phys, "/input0", sizeof(mouse->phys));

  input_dev-> name = "mOUSE LOUCO";  
  
  usb_to_input_id(device, &input_dev->id);
  input_dev->dev.parent = &interface->dev;

  input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
  input_dev->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) |
	BIT_MASK(BTN_RIGHT) | BIT_MASK(BTN_MIDDLE);
  input_dev->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
  input_dev->keybit[BIT_WORD(BTN_MOUSE)] |= BIT_MASK(BTN_SIDE) |
 	BIT_MASK(BTN_EXTRA);
  input_dev->relbit[0] |= BIT_MASK(REL_WHEEL);
  
  input_set_drvdata(input_dev, mouse); 
  input_dev ->open = mouse_open;
  input_dev->close = mouse_close;

  usb_fill_int_urb(mouse->irq,device,pipe, mouse->data,(maxp > 8 ? 8 : maxp),usb_mouse_irq, mouse, endpoint->bInterval);
  
  mouse->irq->transfer_dma = mouse->data_dma;
  mouse->irq->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;

  usb_set_intfdata(interface, mouse);
  return 0;
}

/* Called on device remove */
static void mouse_disconnect(struct usb_interface *interface){
  usb_deregister_dev(interface, &usb_class);
  dev_info(&interface->dev, "USB Mouse now disconnected\n");
}

/* Identifies USB interface driver to usbcore */
static struct usb_driver mouse_driver = {
  name:         "mymouse",
  id_table:     mouse_table,
  probe:        mouse_probe,
  disconnect:   mouse_disconnect,
};

/* Basic init and exit of the driver */
#ifdef MODULE
static int __init mouse_init(void){
  printk("mouse: Mouse Init");
  int retval;

  if((retval = usb_register(&mouse_driver)))
    printk(KERN_ERR "mouse: usb_register failed. Error number %d", retval);

  return retval;
}

static void __exit mouse_exit(void){
  printk("mouse: Mouse Exit");
  /* Deregister this driver with the USB subsystem */
  usb_deregister(&mouse_driver);
}
#endif

/* Adding the module to kernel */
module_init(mouse_init);
module_exit(mouse_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("USB Mouse Driver");

