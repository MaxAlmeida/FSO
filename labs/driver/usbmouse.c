#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/device.h>

#define BUF_LEN 18

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

/* Called when a process tries to open the device file
 * like "cat /dev/FILE" */
static int mouse_open(
    struct inode *ind,
    struct file *file){

  if(device_open){
    printk(KERN_ERR "[%d]Busy!\n", device_open);
    return -EBUSY;
  }

  device_open++;
  sprintf(msg, "[%d]Mouse opened\n", counter++);
  msg_ptr = msg;

  return 0; /* Return Success */
}

/* Called when a process closes the device file */
static int mouse_close(
    struct inode *ind,
    struct file *file){
  printk(KERN_INFO "[%d]Mouse closed\n", counter);
  device_open--; /* Release the file to the next call */
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

  printk(KERN_INFO "mouse: mouse_probe: starting\n");
  device = interface_to_usbdev(interface);

  int retval = usb_register_dev(interface, &usb_class);
  /* Try to register the device */
  if(retval < 0){
    printk(KERN_ERR "mouse: Not able to get a minor for this device. Error %d", retval);
    return retval;
  }else{
    printk(KERN_INFO "mouse: Minor obtained: %d\n", interface->minor);
  }

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

