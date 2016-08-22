NF := my_chardev
obj-m += $(NF).o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm modules.order Module.symvers $(NF).ko $(NF).mod.c $(NF).mod.o $(NF).o \
	.$(NF).ko.cmd .$(NF).mod.o.cmd .$(NF).o.cmd -R .tmp_versions
