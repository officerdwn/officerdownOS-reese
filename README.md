# officerdownOS-reese

This fork drops the original BIOS mode and runs only as a UEFI application.

## Building

Install the `gnu-efi` development package and run `make` to produce
`officerdownOS.efi`. Booting this image will start the original
text-based kernel in the UEFI console.

## Running with QEMU

You can test the binary using the OVMF firmware that ships with most
Linux distributions:

```bash
qemu-system-x86_64 \
    -drive if=pflash,format=raw,readonly=on,file=/usr/share/OVMF/OVMF_CODE_4M.fd \
    -drive if=pflash,format=raw,file=/usr/share/OVMF/OVMF_VARS_4M.fd \
    -drive format=raw,file=fat:rw:.
```
