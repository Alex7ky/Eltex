NF := my_ioctl_dev
obj-m += $(NF).o

KDIR := /usr/src/linux-headers-$(shell uname -r)
PWD := $(shell pwd)

check:
	/usr/src/linux-headers-4.4.0-34-generic/scripts/checkpatch.pl --no-tree -f $(NF).c include/$(NF).h

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.order *.symvers *.mod.c .chdev.* .tmp_versions .*.cmd
