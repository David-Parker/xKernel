C_SOURCES = $(wildcard kernel/*.c) $(wildcard kernel/**/*.c)
ASM_SOURCES = $(wildcard kernel/*.s) $(wildcard kernel/**/*.s)
# Nice syntax for file extension replacement
OBJ = ${ASM_SOURCES:.s=.o} ${C_SOURCES:.c=.o}
BINDIR = ./bin/x86

# Change this if your cross-compiler is somewhere else
CC = /usr/local/i686elfgcc/bin/i686-elf-gcc
GDB = gdb
LD = /usr/local/i686elfgcc/bin/i686-elf-ld
INC=-I./kernel/include
# -g: Use debugging symbols in gcc
CFLAGS = -g -std=c99 -Os

DIRS=${BINDIR} ${BINDIR}/iso
$(shell rm -rf $(DIRS))
$(shell mkdir -p $(DIRS))

# First rule is run by default
os-image.bin: boot/mbr.bin boot/bootloader.bin kernel.bin kernel.elf
	find . -type f -name "*.o" -exec mv -i {} ${BINDIR} \;
	find . -type f -name "*.bin" -exec mv -i {} ${BINDIR} \;
	find . -type f -name "*.img" -exec mv -i {} ${BINDIR} \;
	cat ${BINDIR}/mbr.bin ${BINDIR}/bootloader.bin ${BINDIR}/kernel.bin > ${BINDIR}/os-image.bin
	truncate -s 64K ${BINDIR}/os-image.bin

# Installs tool dependencies then builds a cross compiler for i686 32-bit architecture
packages:
	sudo apt install qemu qemu-kvm virt-manager curl nasm libgmp3-dev libmpfr-dev libmpc-dev texinfo gcc g++
	sudo sh ./install.sh

virt: iso
	sudo qemu-system-x86_64 -m 2g -enable-kvm -cpu host,+tsc,+msr,+invtsc -cdrom ${BINDIR}/iso/xkernel.iso
	#sudo qemu-system-x86_64 -enable-kvm -cpu max -cdrom ${BINDIR}/iso/myos.iso
	#-drive file=${BINDIR}/os-image.bin,index=0,media=disk,format=raw

iso: os-image.bin
	dd if=/dev/zero of=${BINDIR}/iso/xkernel.img bs=1024 count=2880
	dd if=${BINDIR}/os-image.bin of=${BINDIR}/iso/xkernel.img seek=0 conv=notrunc
	genisoimage -quiet -V 'XKERNEL' -input-charset iso8859-1 -o ${BINDIR}/iso/xkernel.iso -b xkernel.img -hide xkernel.img ${BINDIR}/iso/
	# -no-emul-boot -boot-load-size 64 -boot-info-table

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -o ${BINDIR}/$@ -Ttext 0x1200 $^ --oformat binary -L/usr/local/i686elfgcc/lib/gcc/i686-elf/9.3.0 -lgcc

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} -o ${BINDIR}/$@ -Ttext 0x1200 $^ -L/usr/local/i686elfgcc/lib/gcc/i686-elf/9.3.0 -lgcc

run: os-image.bin
	qemu-system-x86_64 -m 2g -drive file=${BINDIR}/os-image.bin,index=0,media=disk,format=raw

build-test:
	$(eval override CFLAGS += -D=UNIT_TEST)

test: build-test run

test-debug: build-test debug

qemu: os-image.bin
	qemu-system-x86_64 -s -S -m 2g -fda ${BINDIR}/os-image.bin

qemu-test: build-test qemu

# Open the connection to qemu-system-x86_64 and load our kernel-object file with symbols
debug: os-image.bin
	qemu-system-x86_64 -m 2g -s -S -fda ${BINDIR}/os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file $(BINDIR)/kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c
	${CC} ${CFLAGS} ${INC} -ffreestanding -c $< -o $@

%.o: %.s
	nasm $< -f elf -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.bin" -delete
	find . -type f -name "*.img" -delete
	find . -type f -name "*.elf" -delete
	rm -rf bin