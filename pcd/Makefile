obj-m = pcd.o

KERN = /lib/modules/$(shell uname -r)/build
PWD = $(shell pwd)

all:
	make -C $(KERN) M=$(PWD) modules
clean:
	all:
	mske -C $(KERN) M=$(PWD) clean

