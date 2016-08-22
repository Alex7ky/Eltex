#ifndef _MY_IOCTL_DEV_H_
#define _MY_IOCTL_DEV_H_

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>

#define DEVICE_NAME "mydev"
#define BUF_LEN 100

#define IOC_MAGIC 'g'
#define OP_READ   _IOR(IOC_MAGIC, 0, char *)
#define OP_WRITE  _IOW(IOC_MAGIC, 1, char *)

static int init_my_ioctl_dev(void);
static void cleanup_my_ioctl_dev(void);
static ssize_t my_ioctl(struct file *, unsigned int, unsigned long);

static char buf[BUF_LEN];

static const struct file_operations fops = {
	.unlocked_ioctl = my_ioctl
};

static struct miscdevice my_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name  = DEVICE_NAME,
	.fops  = &fops,
	.mode  = 0666
};

#define DRIVER_AUTHOR "Alexandr Stolpovsky <stolpovsky7@yandex.ru>"
#define DRIVER_DESC   "A sample driver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);

#endif
