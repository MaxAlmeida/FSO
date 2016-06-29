#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/err.h>

#define MIN(a,b) (((a)<=(b))?(a):(b))
#define BULK_EP_IN 0x81
#define MAX_PKT_SIZE 512

static struct usb_device *device;
static struct usb_class_driver class;
static unsigned char bulk_buf[MAX_PKT_SIZE];

/* Open device file */
static int mouse_open(
    struct inode *ind,
    struct file *file){
  return 0;
}

/* Close device file */
static int mouse_close(
    struct inode *ind,
    struct file *file){
  return 0;
}

/* Reading data from the device */
static ssize_t mouse_read(
    struct file *file,
    char __user *buf,
    size_t cnt,
    loff_t *off){

  int retval;
  int read_cnt;

  /* Read the data from the bulk endpoint */
  retval = usb_bulk_msg(
      device,
      usb_rcvbulkpipe(device, BULK_EP_IN),
      bulk_buf,
      MAX_PKT_SIZE,
      &read_cnt,
      5000);

  if(retval){
     printk(KERN_ERR "Bulk message returned %d\n",retval);
     return retval;
  }
  if(copy_to_user(buf, bulk_buf, MIN(cnt,read_cnt)))
    return -EFAULT;

  return MIN(cnt, read_cnt);
}

/* Write data to the bulk endpoint. Not usefull for a mouse */
static ssize_t mouse_write(
     struct file *file,
     const char __user *buf,
     size_t cnt,
     loff_t *off){
  return -EINVAL;
}

/* File operation structs */
static struct file_operations fops = {
  open: mouse_open,
  release: mouse_close,
  read: mouse_read,
  write: mouse_write,
};

/* Called on device insertion. Only if no other driver has loaded */
static int mouse_probe(
    struct usb_interface *interface,
    const struct usb_device_id *id){

  struct usb_host_interface *iface_desc = interface->cur_altsetting;
  struct usb_endpoint_descriptor *endpoint;
  int retval;
  device = interface_to_usbdev(interface);
  class.name = "usb/mouse%d";
  class.fops = &fops;

  if((retval = usb_register_dev(interface, &class)) < 0)
    printk(KERN_ERR "mouse: Not able to get a minor for this device. Error %d", retval);
  else
    printk(KERN_INFO "mouse: Minor obtained: %d\n", interface->minor);

  return 0;
}

/* Called on device remove */
static void mouse_disconnect(
    struct usb_interface *interface){

  printk(KERN_INFO "mouse: Mouse i/f %d now disconnected\n",
      interface->cur_altsetting->desc.bInterfaceNumber);
  usb_deregister_dev(interface, &class);
}


/* Define witch device are plug-in */
static struct usb_device_id mouse_table[]={
  {USB_DEVICE(0x046d, 0xc534)}, /* (idVendor, idProduct) */
  {} /* Terminating Entry */
};

MODULE_DEVICE_TABLE(usb,mouse_table);

/* Identifies USB interface driver to usbcore */
static struct usb_driver mouse_driver = {
  name: "mymouse",
  id_table: mouse_table,
  probe: mouse_probe,
  disconnect: mouse_disconnect,
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

