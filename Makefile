# UEFI-only build
CC = gcc
LD = ld

CFLAGS = -I/usr/include/efi -I/usr/include/efi/x86_64 \
    -DEFI_FUNCTION_WRAPPER -fshort-wchar -mno-red-zone -fpie \
    -fno-stack-protector -fno-asynchronous-unwind-tables

OBJS = uefi_main.o
TARGET = officerdownOS.efi

all: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) -nostdlib -znocombreloc -T /usr/lib/elf_x86_64_efi.lds \
	    -shared -Bsymbolic $^ /usr/lib/crt0-efi-x86_64.o \
	    -L/usr/lib -lefi -lgnuefi -o officerdownOS.so
	objcopy -j .text -j .sdata -j .data -j .dynamic -j .dynsym \
	    -j .rel -j .rela -j .reloc -j .eh_frame -O efi-app-x86_64 \
	    officerdownOS.so $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f *.o *.so *.efi
	@echo "All build artifacts removed."
