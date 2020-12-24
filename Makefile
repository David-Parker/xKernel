C_SOURCES = $(wildcard kernel/*.c) $(wildcard kernel/**/*.c)
ASM_SOURCES = $(wildcard kernel/*.s) $(wildcard kernel/**/*.s)
HEADERS = $(wildcard kernel/*.h) $(wildcard kernel/**/*.h)
# Nice syntax for file extension replacement
OBJ = ${ASM_SOURCES:.s=.o} ${C_SOURCES:.c=.o}
BINDIR = ./bin/x86

# Change this if your cross-compiler is somewhere else
CC = /usr/local/i386elfgcc/bin/i386-elf-gcc
GDB = gdb
LD = /usr/local/i386elfgcc/bin/i386-elf-ld
INC=-I./
# -g: Use debugging symbols in gcc
CFLAGS = -g -std=c99

DIRS=${BINDIR} ${BINDIR}/iso
$(shell rm -rf $(DIRS))
$(shell mkdir -p $(DIRS))

# First rule is run by default
os-image.bin: boot/bootloader.bin kernel.bin kernel.elf
	find . -type f -name "*.o" -exec mv -i {} ${BINDIR} \;
	find . -type f -name "*.bin" -exec mv -i {} ${BINDIR} \;
	find . -type f -name "*.img" -exec mv -i {} ${BINDIR} \;
	cat ${BINDIR}/bootloader.bin ${BINDIR}/kernel.bin > ${BINDIR}/os-image.bin
	truncate -s 32K ${BINDIR}/os-image.bin

packages:
	sudo apt-get install qemu-kvm qemu virt-manager virt-viewer libvirt-bin bcc

virt: iso
	sudo qemu-system-i386 -enable-kvm -cpu host,+tsc,+msr,+invtsc -cdrom ${BINDIR}/iso/xkernel.iso
	#sudo qemu-system-i386 -enable-kvm -cpu max -cdrom ${BINDIR}/iso/myos.iso
	#-drive file=${BINDIR}/os-image.bin,index=0,media=disk,format=raw

iso: os-image.bin
	dd if=/dev/zero of=${BINDIR}/iso/xkernel.img bs=1024 count=2880
	dd if=${BINDIR}/os-image.bin of=${BINDIR}/iso/xkernel.img seek=0 conv=notrunc
	genisoimage -quiet -V 'XKERNEL' -input-charset iso8859-1 -o ${BINDIR}/iso/xkernel.iso -b xkernel.img -hide xkernel.img ${BINDIR}/iso/
	# -no-emul-boot -boot-load-size 64 -boot-info-table

# '--oformat binary' deletes all symbols as a collateral, so we don't need
# to 'strip' them manually on this case
kernel.bin: boot/kernel_entry.o ${OBJ}
	${LD} -o ${BINDIR}/$@ -Ttext 0x1000 $^ --oformat binary -L/usr/local/i386elfgcc/lib/gcc/i386-elf/4.9.1 -lgcc

# Used for debugging purposes
kernel.elf: boot/kernel_entry.o ${OBJ}
	${LD} -o ${BINDIR}/$@ -Ttext 0x1000 $^ -L/usr/local/i386elfgcc/lib/gcc/i386-elf/4.9.1 -lgcc

run: os-image.bin
	qemu-system-i386 -fda ${BINDIR}/os-image.bin

build-test:
	$(eval override CFLAGS += -D=UNIT_TEST)

test: build-test os-image.bin
	qemu-system-i386 -fda ${BINDIR}/os-image.bin

test-debug: build-test debug

# Open the connection to qemu and load our kernel-object file with symbols
debug: os-image.bin
	qemu-system-i386 -s -S -fda ${BINDIR}/os-image.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file $(BINDIR)/kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} ${INC} -ffreestanding -c $< -o $@

%.o: %.s ${HEADERS}
	nasm $< -f elf -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	find . -type f -name "*.o" -delete
	find . -type f -name "*.bin" -delete
	find . -type f -name "*.img" -delete
	rm -rf bin