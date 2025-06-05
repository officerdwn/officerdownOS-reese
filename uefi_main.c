#include <efi.h>
#include <efilib.h>

EFI_STATUS
efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
    InitializeLib(ImageHandle, SystemTable);
    Print(L"officerdownOS UEFI boot\n");
    Print(L"Press any key to exit...\n");

    EFI_INPUT_KEY key;
    uefi_call_wrapper(SystemTable->ConIn->Reset, 2, SystemTable->ConIn, FALSE);
    while (uefi_call_wrapper(SystemTable->ConIn->ReadKeyStroke, 2,
                             SystemTable->ConIn, &key) == EFI_NOT_READY) {
    }
    return EFI_SUCCESS;
}
