PWD := $(shell pwd)
KVERSION := $(shell uname -r)
KVERSION_DIR = /usr/src/linux-headers-$(KVERSION)/

MODULE_NAME = platform_driver
obj-m := $(MODULE_NAME).o

all:
	make -C $(KVERSION_DIR) M=$(PWD) modules
clean:
	make -C $(KVERSION_DIR) M=$(PWD) clean
