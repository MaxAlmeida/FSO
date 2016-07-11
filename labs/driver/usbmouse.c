#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/usb/input.h>

#define VENDOR_ID   0x15d9
#define PRODUCT_ID  0x0a4c
#define MOUSE_NAME  "Custom_Mouse"

/* Called when a process tries to open the device file
 * like "cat /dev/FILE" */
static int mouse_open(struct input_dev *input);
static void mouse_close(struct input_dev *input);
/* Called on device insertion
 * Contains all the initializations of the driver */
static int mouse_probe(struct usb_interface *interface, const struct usb_device_id *id);

/* Called on device remotion
 * Contains all the dealocation of the driver */
static void mouse_disconnect(struct usb_interface *interface);

/* Table of devices that work with this driver */
static struct usb_device_id mouse_table [] = {
  { USB_DEVICE(VENDOR_ID, PRODUCT_ID)  },
  {  } /* Terminating entry */
}; MODULE_DEVICE_TABLE (usb, mouse_table);

/* Define the file operations functions */
static struct file_operations fops = {
  open:       mouse_open,
};

/* identifies a USB driver that wants to use the USB major number */
static struct usb_class_driver mouse_class = {
  name: "usb/mouse%d",
  fops: &fops,
};

/* Create a value mouse_driver structure */
static struct usb_driver mouse_driver = {
  name:       MOUSE_NAME,
  id_table:   mouse_table,
  probe:      mouse_probe,
  disconnect: mouse_disconnect,
};

/* Defining a local device structure
 * One structure for each connected device */
struct usb_mouse {
  char name[128];
  char phys[64]; 
  int *int_in_buffer;
  int int_in_size;
  int int_in_endpointAddr;
  struct urb* irq;
  signed char *data;
  struct usb_device *udev;
  struct input_dev *input;
};

#ifdef MODULE
static int __init mouse_init(void){
  int retval = 0;

  /* Register this driver with USB subsystem */
  retval = usb_register(&mouse_driver);
  if(retval)
    printk(KERN_ERR "mouse: usb_register failed. ERROR: %d\n", retval);

  return retval;
}

static void __exit mouse_exit(void){
  /* Deregister this driver with USB subsystem */
  usb_deregister(&mouse_driver);
}
#endif

/* Adding the module to kernel */
module_init(mouse_init);
module_exit(mouse_exit);


/* Definition of the functions */

static void mouse_callback(struct urb *urb){
  struct usb_mouse *mouse = urb->context;
  signed char *data = mouse->data;
  struct input_dev *input = mouse->input;
   
  if (urb->status &&
      !(urb->status == -ENOENT ||
        urb->status == -ECONNRESET ||
        urb->status == -ESHUTDOWN)) {
    printk(KERN_ERR "%s - nonzero int in status received: %d",
        __FUNCTION__, urb->status);
  }
  printk(KERN_INFO "Mouse callback!\n");
  
  input_report_key(input, BTN_LEFT, data[0] & 0x01);
  input_report_key(input, BTN_RIGHT, data[0] & 0x02);
  input_report_rel(input, REL_X, data[1]);
  input_report_rel(input, REL_Y, data[2]);
  input_sync(input);
  /* free up our allocated buffer */
  //usb_free_coherent(urb->dev, urb->transfer_buffer_length,
     // urb->transfer_buffer, urb->transfer_dma);
}

static int mouse_probe(
    struct usb_interface *interface,
    const struct usb_device_id *id){

  int i = 0;
  int retval = 0;
  int buffer_size;
  char *buff;
  char *user_buffer;

  struct urb *mouse_urb;
  struct usb_mouse *dev;
  struct usb_host_interface *iface_desc;
  struct usb_endpoint_descriptor *endpoint;
 
  
  

  /* Set up the endpoint informations */
  dev = kmalloc(sizeof(struct usb_mouse),GFP_KERNEL);
  dev->input = input_allocate_device();
  printk("Printando aqui o input device \n %d", dev->input);
  iface_desc = interface->cur_altsetting;
  for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
    endpoint = &iface_desc->endpoint[i].desc;
    if(((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN)
        && ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)){
      /* We found a interrupt endpoint */
      buffer_size = endpoint->wMaxPacketSize;
      dev->int_in_size = buffer_size;
      dev->int_in_endpointAddr = endpoint->bEndpointAddress;
      dev->int_in_buffer = kmalloc(buffer_size, GFP_KERNEL);
      if(!dev->int_in_buffer){
        printk(KERN_ERR "Could not allocate int_in_buffer\n");
        goto error;
      }
    }
  }
  if(!(dev->int_in_endpointAddr)){
     printk(KERN_ERR "Could not find int-in\n");
     goto error;
  }

  /*Set up input device */
 /*
  printk("Configurando input..");
  
  if(dev->udev->manufacturer)
	strlcpy(dev->name, dev->udev->manufacturer, sizeof(dev->name));
  if(dev->udev->product){
	if (dev->udev->manufacturer)
		strlcat(dev->name, " ",sizeof(dev->name));
        strlcat(dev->name, dev->udev->product, sizeof(dev->name));

   }
   if (!strlen(dev->name))
	snprintf(dev->name, sizeof(dev->name),"USB HIDBP MOUSE %04x:%04x",
	le16_to_cpu(dev->udev->descriptor.idVendor),
        le16_to_cpu(dev->udev->descriptor.idProduct));
  
   //usb_make_path(dev, dev->phys, sizeof(dev->phys));
   strlcat(dev->phys, "/input0", sizeof(dev->phys));

  dev->input->name = dev->name;
  dev->input->phys = dev->phys;
  //usb_to_input_id(dev->udev, &dev->input->id);
  dev->input->dev.parent = &interface->dev;  	 
  */

  /*Calls the procedures below in probe function*/
  struct input_dev *input; 	 
  input->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_REL);
  input->keybit[BIT_WORD(BTN_MOUSE)] = BIT_MASK(BTN_LEFT) | BIT_MASK(BTN_RIGHT);
  input->relbit[0] = BIT_MASK(REL_X) | BIT_MASK(REL_Y);
  input->open = mouse_open;
  input->close = mouse_close;
  input_register_device(input); 

  /*Calls the procedure below in disconect or exit device function*/
  input_unregister_device(input); 

  /* Set up the device */
  dev->udev = interface_to_usbdev(interface);   

/* Save our data pointer in this interface device */
  usb_set_intfdata(interface,dev);

  /* We can register the device now, as it is ready */
  retval = usb_register_dev(interface, &mouse_class);
  if (retval) {
    /* something prevented us from registering this driver */
    printk(KERN_ERR "Not able to get a minor for this device\n");
    usb_set_intfdata(interface, NULL);
    goto error;
  }

  printk(KERN_INFO "Set up URB\n");
  mouse_urb = usb_alloc_urb(0, GFP_KERNEL);
  if (!mouse_urb){
    retval = -ENOMEM;
    printk(KERN_ERR "Could not alloc urb. ERROR: %d\n",retval);
    goto error;
  }

  /* Buffer to send data */
  buff = usb_alloc_coherent(dev->udev, sizeof(buff), GFP_KERNEL, &mouse_urb->transfer_dma);
  if(!buff){
    retval = -ENOMEM;
    printk(KERN_ERR "Could not alloc buffer. ERROR: %d\n",retval);
    goto error;
  }

  /* Initialize the URB */
  usb_fill_int_urb(mouse_urb, dev->udev,
    usb_rcvintpipe(dev->udev, dev->int_in_endpointAddr),
    buff, sizeof(buff), mouse_callback, dev, 10);

  printk("Registrando device");
  int error = input_register_device(dev->input);
	if (error) {
		printk(KERN_ERR "button.c: Failed to register device\n");
		//goto err_free_dev;
	}
        else
                printk("\nConsegui registar o device");
  printk("Submetendo urb");
  retval = usb_submit_urb(mouse_urb, GFP_KERNEL);
  if(retval){
     printk(KERN_ERR "Failed submitting write URB. ERROR: %d\n", retval);
     goto error;
  }

  printk(KERN_INFO "USB Mouse #%d now connected!\n", interface->minor);

error:
  return retval;
}

static void mouse_disconnect(
    struct usb_interface *interface){

  struct usb_mouse *dev;
  int minor = interface->minor;

  /* Free pointers */
  dev = usb_get_intfdata(interface);
  usb_set_intfdata(interface, NULL);

  /* Give back our minor */
  usb_deregister_dev(interface, &mouse_class);

  printk(KERN_INFO "USB Mouse #%d now disconnected!\n", minor);
}

static int mouse_open(struct input_dev *input){
  printk("Entrei na open\n");
  struct usb_mouse *mouse = input_get_drvdata(input);
  mouse->irq->dev = mouse->input;
  if(usb_submit_urb(mouse->irq, GFP_KERNEL))
  {
	return -EIO;
      printk("Entrei no if\n");
  }
  else
       printk("Abri esse carai\n");
 
   return 0;
}

static void mouse_close(struct input_dev *input){
  struct usb_mouse *mouse = input_get_drvdata(input);
  usb_kill_urb(mouse->irq);
  
}
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("USB Mouse Driver");
