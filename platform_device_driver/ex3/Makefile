PWD := $(shell pwd)
KVERSION := $(shell uname -r)
KVERSION_DIR = /usr/src/linux-headers-$(KVERSION)/

MODULE_NAME = platform_device_driver
multiple_src-objs := $(MODULE_NAME).o printa.o
obj-m := multiple_src.o

all:
	make -C $(KVERSION_DIR) M=$(PWD) modules
clean:
	make -C $(KVERSION_DIR) M=$(PWD) clean
