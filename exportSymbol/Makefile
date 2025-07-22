obj-m = avg.o add.o

KERN = /lib/modules/$(shell uname -r)/build
PWD= $(shell pwd)


all:
	make -C $(KERN) M=$(PWD) modules 

clean:
	make -C $(KERN) M=$(PWD) clean 
