NF := hook_udp_packet
obj-m += $(NF).o

KDIR := /usr/src/linux-headers-$(shell uname -r)
PWD := $(shell pwd)

check:
	/usr/src/linux-headers-$(shell uname -r)/scripts/checkpatch.pl --no-tree -f $(NF).c include/$(NF).h

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.order *.symvers *.mod.c .chdev.* .tmp_versions .*.cmd
