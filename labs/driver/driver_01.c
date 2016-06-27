#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/usb.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Paulo Tada <paulohtfs@gmail.com>");
MODULE_DESCRIPTION("USB Mouse Driver");

static struct usb_device *device;

static int mouse_probe(
    struct usb_interface *interface,
    const struct usb_device_id *id){

  struct usb_host_interface *iface_desc;
  struct usb_endpoint_descriptor *endpoint;
  iface_desc = interface->cur_altsetting;

  printk(KERN_INFO "mouse: Mouse i/f %d now probed: (%04X:%04X)\n",
      iface_desc->desc.bInterfaceNumber,
      id->idVendor,
      id->idProduct);
  printk(KERN_INFO "mouse: ID->bNumEndpoints: %02X\n",
      iface_desc->desc.bNumEndpoints);
  printk(KERN_INFO "mouse: ID->bInterfaceClass: %02X\n",
      iface_desc->desc.bInterfaceClass);

  int i;
  for(i=0;i<iface_desc->desc.bNumEndpoints; i++){
    endpoint = &iface_desc->endpoint[i].desc;
    printk(KERN_INFO "mouse: ED [%d]->bEndpointAddress: 0x%02X\n",
        i, endpoint->bEndpointAddress);
    printk(KERN_INFO "mouse: ED [%d]->bmAttributes: 0x%02X\n",
        i, endpoint->bmAttributes);
    printk(KERN_INFO "mouse: ED [%d]->WMaxPacketSize: 0x%04X\n",
        i, endpoint->wMaxPacketSize);
  }

  device = interface_to_usbdev(interface);
  return 0;
}

static void mouse_disconnect(
    struct usb_interface *interface){

  printk(KERN_INFO "mouse: Mouse i/f %d now disconnected\n",
      interface->cur_altsetting->desc.bInterfaceNumber);
}

static struct usb_device_id mouse_table[]={
  {USB_DEVICE(0x1516, 0x8628)},
  {} /* Terminating Entry */
};

MODULE_DEVICE_TABLE(usb,mouse_table);

/* Identifies USB interface driver to usbcore */
static struct usb_driver mouse_driver = {
  name: "My USB Mouse",
  id_table: mouse_table,
  probe: mouse_probe,
  disconnect: mouse_disconnect,
};

#ifdef MODULE
/* Basic init and exit of the driver */
static int __init mouse_init(void){
  printk("mouse: Mouse Init");
  return usb_register(&mouse_driver);
}

static void __exit mouse_exit(void){
  printk("mouse: Mouse Exit");
  usb_deregister(&mouse_driver);
}
#endif

module_init(mouse_init);
module_exit(mouse_exit);

