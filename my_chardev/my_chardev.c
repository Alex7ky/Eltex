# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/proc_fs.h>
# include <asm/uaccess.h>
# include <linux/slab.h>

#define DRIVER_AUTHOR "Alexandr Stolpovsky <stolpovsky7@yandex.ru>"
#define DRIVER_DESC   "A sample driver"

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);

static int init_my_chardev(void);
static void cleanup_my_chardev(void);
static int mydevice_open(struct inode *, struct file *);
static int mydevice_release(struct inode *, struct file *);
static ssize_t mydevice_read(struct file *, char *, size_t, loff_t *);
static ssize_t mydevice_write(struct file *, const char *, size_t, loff_t *);

#define SUCCES 0
#define DEVICE_NAME "mydev"
#define BUF_LEN 20

static dev_t Major;
static int Device_Status;
static char msg[BUF_LEN];
static char *msg_Ptr;

static const struct file_operations fops = {
	.open = mydevice_open,
	.release = mydevice_release,
	.read = mydevice_read,
	.write = mydevice_write
};

static int init_my_chardev(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);

	if (Major < 0) {
		pr_info("register_chrdev() error %d\n", Major);
		return Major;
	}
	pr_info("Major number %d\n", Major);
	pr_info("Please create a device file with\n");
	pr_info("'mknod -m 0666 /dev/mydev c %d 0'.\n", Major);

	return SUCCES;
}

static void cleanup_my_chardev(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
}

static int mydevice_open(struct inode *inode, struct file *file)
{
	if (Device_Status)
		return -EBUSY;

	Device_Status++;
	msg_Ptr = msg;
	try_module_get(THIS_MODULE);
	pr_info("mydevice_open() end!\n");

	return SUCCES;
}

static int mydevice_release(struct inode *inode, struct file *file)
{
	Device_Status--;
	module_put(THIS_MODULE);
	pr_info("mydevice_release() end!\n");

	return SUCCES;
}

static ssize_t
mydevice_read(struct file *filp, char *buffer, size_t length, loff_t *offset)
{
	int bytes_read = 0;

	if (*msg_Ptr == 0)
		return 0;

	while (length && *msg_Ptr) {
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;
	}
	pr_info("mydevice_read() end!\n");

	return bytes_read;
}


static ssize_t mydevice_write(struct file *file, const char __user *buffer,
	size_t len, loff_t *off)
{
	int bwrite;

	for (bwrite = 0; bwrite < len && bwrite < BUF_LEN; bwrite++)
		get_user(msg[bwrite], buffer + bwrite);

	msg_Ptr = msg;
	pr_info("%s", msg);
	pr_info("mydevice_write() end!\n");

	return bwrite;
}

module_init(init_my_chardev);
module_exit(cleanup_my_chardev);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");
