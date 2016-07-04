#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define VENDOR_ID   0x046d
#define PRODUCT_ID  0xc534
#define MOUSE_NAME  "Custom_Mouse"

/* table of devices that work with this driver */
static struct usb_device_id mouse_table [] = {
  { USB_DEVICE(VENDOR_ID, PRODUCT_ID)  },
  {  } /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, mouse_table);

/* Called on device insertion
 * Contains all the initializations of the driver */
static int mouse_probe(struct usb_interface *interface, const struct usb_device_id *id);

/* Called on device remotion
 * Contains all the dealocation of the driver */
static void mouse_disconnect(struct usb_interface *interface);

/* Create a value mouse_driver structure */
static struct usb_driver mouse_driver = {
  name:       MOUSE_NAME,
  id_table:   mouse_table,
  probe:      mouse_probe,
  disconnect: mouse_disconnect,
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
static int mouse_probe(
    struct usb_interface *interface,
    const struct usb_device_id *id){

  int i = 0;
  struct usb_host_interface *iface_desc;
  struct usb_endpoint_descriptor *endpoint;

  /* Set up the endpoint informations */
  iface_desc = interface->cur_altsetting;
  for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
    endpoint = &iface_desc->endpoint[i].desc;
    if(((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN)
        && ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)){
      /* We found a interrupt endpoint */
    }
  }
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("USB Mouse Driver");
