obj-m := mouse.o
all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	sudo insmod mouse.ko

remove:
	sudo rmmod mouse

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
