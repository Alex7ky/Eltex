#include "my_ioctl_dev.h"

static int init_my_ioctl_dev(void)
{
	static int status;

	status = misc_register(&my_misc_device);

	if (status < 0) {
		pr_info("misc_register failed with %d\n", status);
		return status;
	}

	return 0;
}

static void cleanup_my_ioctl_dev(void)
{
	misc_deregister(&my_misc_device);
}


static ssize_t
my_ioctl(struct file *fi, unsigned int cmd, unsigned long arg)
{
	char *buf = kzalloc(BUF_LEN, GFP_KERNEL);
	char my_str[] = "hello user, I'm kernel!";

	switch (cmd) {
	case OP_WRITE:
		if (copy_to_user((char *)arg, my_str, BUF_LEN)) {
			pr_info("err copy to user!\n");
			return -1;
		}

		pr_info("write: %s", (char *)arg);
		break;

	case OP_READ:
		if (copy_from_user(buf, (char *)arg, BUF_LEN)) {
			pr_info("err copy from user!\n");
			return -1;
		}

		pr_info("read: %s", buf);
		break;

	default:
		pr_info("error cmd!\n");
		return -1;
	}
	pr_info("my_ioctl_dev() end!\n");

	return 0;
}

module_init(init_my_ioctl_dev);
module_exit(cleanup_my_ioctl_dev);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_SUPPORTED_DEVICE("testdevice");
