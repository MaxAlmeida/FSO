obj-m := $(FILE).o
all:
	@echo 'Usage: make <command>'
	@echo
	@echo 'Commands:'
	@echo '  compile <file.o>	Generates the driver executable'
	@echo '  install <file.ko> 	Install the driver'
	@echo '  remove <DRIVER_NAME> 	Remove the driver'
	@echo '  clean 		Clean the generated files'

compile:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

install:
	sudo rmmod usbhid || true
	sudo insmod $(filter-out $@, $(MAKECMDGOALS)).ko

remove:
	sudo rmmod $(filter-out $@, $(MAKECMDGOALS)) || true

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

%:
	@:
