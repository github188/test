MOBJ = nl_test

ifneq ($(KERNELRELEASE),)
obj-m := nl_test.o
nl_test-objs := nl.o
else
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	$(RM) -rf *.o *~ core .depend .*.cmd *.ko *.mod.c .tmp_versions \
		Module.symvers modules.order
endif
