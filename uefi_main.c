#include <efi.h>
#include <efilib.h>
#include "kernel.h"

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    init_uefi(SystemTable);
    kernel_main();
    return EFI_SUCCESS;
}
