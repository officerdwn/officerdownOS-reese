# UEFI-only build
CC = gcc
LD = ld

CFLAGS = -I/usr/include/efi -I/usr/include/efi/x86_64 \
    -DEFI_FUNCTION_WRAPPER -fshort-wchar -mno-red-zone -fpic \
    -fno-stack-protector -fno-asynchronous-unwind-tables -Wall -Wextra

OBJS = uefi_main.o kernel.o
TARGET = officerdownOS.efi
IMG = officerdownOS.img

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds \
	    -shared -Bsymbolic $^ /usr/lib/crt0-efi-x86_64.o \
	    -L/usr/lib -lefi -lgnuefi -o officerdownOS.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym \
	    -j .rel -j .rela -j .reloc -O efi-app-x86_64 \
	    officerdownOS.so $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Create a FAT32 image with EFI bootloader
.PHONY: img
img: $(TARGET)
	rm -f $(IMG)
	dd if=/dev/zero of=$(IMG) bs=1M count=64
	mkfs.vfat $(IMG)
	mmd -i $(IMG) ::/EFI ::/EFI/BOOT
	mcopy -i $(IMG) $(TARGET) ::/EFI/BOOT/BOOTX64.EFI

# Run in QEMU with split OVMF firmware
.PHONY: run
run: img
	qemu-system-x86_64 \
	  -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE.fd \
	  -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS.fd \
	  -drive format=raw,file=$(IMG)

.PHONY: clean
clean:
	rm -f *.o *.so *.efi *.img
	@echo "All build artifacts removed."
