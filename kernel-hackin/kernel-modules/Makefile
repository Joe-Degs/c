MULTIPLE = hello-multiple-files
obj-m += hello-2/main.o
obj-m += hello-args/main.o
obj-m += hello-info/main.o
obj-m += hello-two/main.o
obj-m += hello-with-data/main.o
obj-m += startstop.o
startstop-objs := $(MULTIPLE)/start.o $(MULTIPLE)/stop.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
