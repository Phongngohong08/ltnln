# Makefile bien dich cac chuong trinh C cua Phan 2
# Su dung:  make        -> bien dich tat ca
#           make clean  -> xoa file thuc thi

CC      = gcc
CFLAGS  = -Wall -O2
TARGETS = quanly_file quanly_tientrinh network server client

all: $(TARGETS)

quanly_file: quanly_file.c
	$(CC) $(CFLAGS) $< -o $@

quanly_tientrinh: quanly_tientrinh.c
	$(CC) $(CFLAGS) $< -o $@

network: network.c
	$(CC) $(CFLAGS) $< -o $@

server: server.c
	$(CC) $(CFLAGS) $< -o $@

client: client.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(TARGETS)
