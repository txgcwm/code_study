#
#
#

ifneq ($(KERNELRELEASE),)
	obj-m := snull.o
else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)
all:
	make -C $(KERNELDIR) M=$(PWD) modules
clean:
	rm -rf *.ko *.o *.mod.o *.mod.c .snull* .depend .tmp_versions *.symvers *.order modules.order  Module.symvers
endif
