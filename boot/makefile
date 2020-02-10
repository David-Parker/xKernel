CFLAGS=-0 -c -ansi
LDFLAGS=-d
ASFLAGS=-f bin

all: bootloader.bin

packages:
	sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-bin bcc

virt: bootloader.iso
	qemu-system-x86_64 -cdrom ../bin/iso/myos.iso -drive file=../bin/bootloader.bin,index=0

bootloader.iso: bootloader.bin
	mkdir -p ../bin/iso
	dd if=/dev/zero of=../bin/iso/bootloader.img bs=1024 count=1440
	dd if=../bin/bootloader.bin of=../bin/iso/bootloader.img seek=0 count=1 conv=notrunc
	genisoimage -quiet -V 'MYOS' -input-charset iso8859-1 -o ../bin/iso/myos.iso -b bootloader.img -hide bootloader.img ../bin/iso/

bootloader.out: bootloader.o
	ld86 -o $@ $^ $(LDFLAGS)

test.o: test.c
	bcc -o $@ $^ $(CFLAGS)

bootloader.bin: bootloader.asm
	mkdir -p ../bin
	nasm -o ../bin/$@ $^ $(ASFLAGS)

clean:
	rm -f *.o bootloader.bin