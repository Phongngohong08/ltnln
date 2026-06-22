# Makefile build module nhan Linux
# Su dung:  make        -> bien dich module (.ko)
#           make clean  -> xoa file build
#
# Yeu cau: da cai goi header cua nhan:
#   sudo apt-get install build-essential linux-headers-$(uname -r)

obj-m += hello_module.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
