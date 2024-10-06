KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)
obj-m := emulmon.o     # same name as the .c file of your module

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
