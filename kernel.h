#ifndef KERNEL_H
#define KERNEL_H

#include <efi.h>
#include <efilib.h>

void init_uefi(EFI_SYSTEM_TABLE *SystemTable);
void kernel_main(void);
void clear_screen(void);
unsigned int print_to_screen(char *string);
void update_cursor(int row, int col);
char get_input(void);
void get_string(char *buffer, int max_length);

#endif
