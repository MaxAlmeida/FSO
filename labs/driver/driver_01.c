#include <linux/module.h>

#define THEMOUSE_BASE 0x2510

MODULE_LICENSE("Dual BSD/GPL");

static struct miscdevice the_mouse = {
  THEMOUSE_MINOR,
  "themouse",
  &our_mouse_fops
}

__init themouse_init(void){
  if(check_region(THEMOUSE_BASE, 3)) return -ENODEV;
  request_region(THEMOUSE_BASE, 3, “themouse”);
  misc_register(&our_mouse);
  return 0;
}

