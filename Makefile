C_SOURCES = $(wildcard kernel/*.c)
HEADERS = $(wildcard kernel/*.h)
# Nice syntax for file extension replacement
OBJ = ${C_SOURCES:.c=.o}
BINDIR = ./bin

# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = gdb
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
# -g: Use debugging symbols in gcc
CFLAGS = -g

DIRS=bin bin/iso
$(shell mkdir -p $(DIRS))

packages:
	sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-bin bcc

virt: myos.iso
	qemu-system-i386 -cdrom ${BINDIR}/iso/myos.iso -drive file=${BINDIR}/os-image.bin,index=0

myos.iso: os-image.bin
	dd if=/dev/zero of=${BINDIR}/iso/myos.img bs=1024 count=1440
	dd if=${BINDIR}/os-image.bin of=${BINDIR}/iso/myos.img seek=0 count=1 conv=notrunc
	genisoimage -quiet -V 'MYOS' -input-charset iso8859-1 -o ${BINDIR}/iso/myos.iso -b myos.img -hide myos.img ${BINDIR}/iso/

# First rule is run by default
os-image.bin: boot/bootloader.bin kernel.bin
	cat $^ > ${BINDIR}/os-image.bin

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ --oformat binary

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} -o $@ -Ttext 0x1000 $^ 

run: os-image.bin
	qemu-system-i386 -fda ${BINDIR}/os-image.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin kernel.elf
	qemu-system-i386 -s -fda os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -rf *.bin *.dis *.o os-image.bin *.elf
	rm -rf kernel/*.o kernel/*.bin boot/*.bin boot/*.o
	rm -rf ${BINDIR}