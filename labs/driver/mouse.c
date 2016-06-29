#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>

#define MOUSE_MINOR 0

/* Interruptions Management */
static int mouse_users = 0;
static int mouse_event = 0;

/* Open file mouse */
static int open_mouse(
    struct inode *inode,
    struct file *file){

  if(mouse_users++)
    return 0;

  return 0;
}

/* Release file mouse */
static int release_mouse(
    struct inode *inode,
    struct file *file){
  return 0;
}

/* Filling in the write handler */
static ssize_t write_mouse(
    struct file *file,
    const char *buffer,
    size_t count,
    loff_t *ppos){
  return -EINVAL;
}

/* Read file mouse */
static ssize_t read_mouse(
     struct file *file,
     char *buffer,
     size_t count,
     loff_t *pos){
  return -EINVAL;
}

/* Definition of the miscdevice */
static struct miscdevice mouse = {
  name: "mymouse",
  minor: MOUSE_MINOR,
};

/* Definition of the file operation */
static struct file_operations fops = {
  read:   read_mouse,
  write:  write_mouse,
  open:   open_mouse,
  release:  release_mouse,
};

/* Init Drive */
static int __init mouse_init(void){
  int retval = misc_register(&mouse);
  if(retval) return retval;

  printk("mouse: Mouse Init\n");
  return 0;
}

/* Exit Drive */
static void __exit mouse_exit(void){
  printk("mouse: Mouse Exit");
  misc_deregister(&mouse);
}

/* Adding to the module */
module_init(mouse_init);
module_exit(mouse_exit);
