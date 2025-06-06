# officerdownOS-reese

This fork drops the original BIOS mode and runs only as a UEFI application.

## Building

Install the `gnu-efi` development package and run `make` to produce
`officerdownOS.efi`. Booting this image will start the original
text-based kernel in the UEFI console.

## Running with QEMU

make sure to build the img with `make img` and use `make run` to run it.
