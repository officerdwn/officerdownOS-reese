#include "kernel.h"
#include "screen.h"
#include <efi.h>
#include <efilib.h>

void display_ui(void);
void display_about(void);
void display_calculator(void);

// Global state
int previous_result = 0;
void init_uefi(EFI_SYSTEM_TABLE *SystemTable) {
    /* gnu-efi sets ST for us */
}


int atoi(const char *str) {
    int result = 0, sign = 1;
    if (*str == '-') {
        sign = -1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result * sign;
}

void itoa(int num, char *str, int base) {
    int i = 0, is_negative = 0;
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    if (num < 0 && base == 10) {
        is_negative = 1;
        num = -num;
    }
    while (num > 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num /= base;
    }
    if (is_negative) {
        str[i++] = '-';
    }
    str[i] = '\0';

    int start = 0, end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start++] = str[end];
        str[end--] = temp;
    }
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

// ------------------------ Core Kernel Logic (Main Loop, Screen Ops) ------------------------

void kernel_main() {
    clear_screen();
    while (1) {
        display_ui();
    }
}

void clear_screen() {
    uefi_call_wrapper(ST->ConOut->ClearScreen, 1, ST->ConOut);
}
unsigned int print_to_screen(char *string) {
    CHAR16 buf[512];
    int len = 0;
    while (string[len] && len < 511) {
        buf[len] = (CHAR16)string[len];
        len++;
    }
    buf[len] = L'\0';
    uefi_call_wrapper(ST->ConOut->OutputString, 2, ST->ConOut, buf);
    return len;
}

void update_cursor(int row, int col) {
    uefi_call_wrapper(ST->ConOut->SetCursorPosition, 3, ST->ConOut, col, row);
}

void get_string(char *buffer, int max_length) {

    int index = 0;
    while (index < max_length - 1) {
        char key = get_input();
        if (key == '\r' || key == '\n') {
            buffer[index] = '\0';
            print_to_screen("\n");
            return;
        } else if (key == '\b') {
            if (index > 0) {
                index--;
                print_to_screen("\b \b");
            }
        } else if (key >= ' ' && key <= '~') {
            buffer[index++] = key;
            char out[2] = {key, 0};
            print_to_screen(out);
        }
    }
    buffer[index] = '\0';
    print_to_screen("\n");
}

char get_input() {
    EFI_INPUT_KEY key;
    UINTN index;
    uefi_call_wrapper(ST->BootServices->WaitForEvent, 3, 1, &ST->ConIn->WaitForKey, &index);
    uefi_call_wrapper(ST->ConIn->ReadKeyStroke, 2, ST->ConIn, &key);
    if (key.UnicodeChar < 0x100)
        return (char)key.UnicodeChar;
    return 0;
}

// ------------------------ UI Screens (Main Menu, About, Calculator) ------------------------

void display_ui() {
    char input_buffer[20];

    clear_screen();

    print_to_screen("                               officerdownOS\n");
    print_to_screen("-------------------------------------------------------------------------------\n");
    print_to_screen("\n");  // extra vertical space
    print_to_screen("    ---------------          ---------------\n");
    print_to_screen("\n");
    print_to_screen("           ?                      2+2=4\n");
    print_to_screen("\n");
    print_to_screen("    ---------------          ---------------\n");
    print_to_screen("\n");
    print_to_screen("   About (Press 'a')      Calculator (Press 'c')\n");
    print_to_screen("\n");
    print_to_screen("Enter your choice: ");

    get_string(input_buffer, sizeof(input_buffer));

    if (strcmp(input_buffer, "a") == 0 || strcmp(input_buffer, "about") == 0) {
        display_about();
    } else if (strcmp(input_buffer, "c") == 0 || strcmp(input_buffer, "calc") == 0) {
        display_calculator();
    } else {
        print_to_screen("\nThis is not recognized. Try again!\n");
        print_to_screen("Press any key to continue...");
        get_input();
        display_ui(); // Redraw the screen
    }
}






void display_about() {
    char about_input[20];

    clear_screen();
    print_to_screen("                                      About\n");
    print_to_screen("-------------------------------------------------------------------------------\n");
    print_to_screen("  officerdownOS Main Branch v0.1.5\n");
    print_to_screen("  officerdownOS Rocky v1.0.0\n");
    print_to_screen("  Committed 05/27/2025\n");
    print_to_screen("\nPress 'b' to go back to the main menu.");

    get_string(about_input, sizeof(about_input));

    if (strcmp(about_input, "b") == 0) {
        display_ui();
    } else {
        print_to_screen("\nInvalid input. Press 'b' to go back.");
    }
}

void display_calculator() {
    char input[10];
    char num1_str[10], num2_str[10];
    int num1, num2, result = 0;

main_menu:
    clear_screen();
    print_to_screen("                                     Office Calculator\n");
    print_to_screen("------------------------------------------------------------------------------------------------\n\n");
    print_to_screen("Calculator version 1.0\n\n");
    print_to_screen("---------------------------\n\n");
    print_to_screen("/////////////////////////////////////\n");

    char prev[32];
    itoa(previous_result, prev, 10);
    print_to_screen("Your previous Calculated number was ");
    print_to_screen(prev);
    print_to_screen("\n/////////////////////////////////////\n\n");

    print_to_screen("Enter the specified alphabet to continue..\n\n");
    print_to_screen("a) Addition\n\n");
    print_to_screen("b) Subtraction\n\n");
    print_to_screen("c) Division\n\n");
    print_to_screen("d) Multiplication\n\n");
    print_to_screen("e) Square, Cube or any power (by Prof. Pickle)\n\n");
    print_to_screen("exit) Exits\n\n");

    print_to_screen("Choice: ");
    get_string(input, sizeof(input));

    if (strcmp(input, "exit") == 0) return;

    if (strcmp(input, "a") == 0 || strcmp(input, "A") == 0) {
        print_to_screen("\nADDITION\n\nnum1: ");
        get_string(num1_str, sizeof(num1_str));
        print_to_screen("       +\nnum2: ");
        get_string(num2_str, sizeof(num2_str));
        num1 = atoi(num1_str);
        num2 = atoi(num2_str);
        result = num1 + num2;
    } else if (strcmp(input, "b") == 0 || strcmp(input, "B") == 0) {
        print_to_screen("\nSUBTRACTION\n\nnum1: ");
        get_string(num1_str, sizeof(num1_str));
        print_to_screen("       -\nnum2: ");
        get_string(num2_str, sizeof(num2_str));
        num1 = atoi(num1_str);
        num2 = atoi(num2_str);
        result = num1 - num2;
    } else if (strcmp(input, "c") == 0 || strcmp(input, "C") == 0) {
        print_to_screen("\nDIVISION\n\nnum1: ");
        get_string(num1_str, sizeof(num1_str));
        print_to_screen("       /\nnum2: ");
        get_string(num2_str, sizeof(num2_str));
        num1 = atoi(num1_str);
        num2 = atoi(num2_str);
        if (num2 == 0) {
            print_to_screen("Error: Division by zero!\n");
            print_to_screen("Press any key to continue...");
            get_input();
            goto main_menu;
        }
        result = num1 / num2;
    } else if (strcmp(input, "d") == 0 || strcmp(input, "D") == 0) {
        print_to_screen("\nMULTIPLICATION\n\nnum1: ");
        get_string(num1_str, sizeof(num1_str));
        print_to_screen("       *\nnum2: ");
        get_string(num2_str, sizeof(num2_str));
        num1 = atoi(num1_str);
        num2 = atoi(num2_str);
        result = num1 * num2;
    } else if (strcmp(input, "e") == 0 || strcmp(input, "E") == 0) {
        print_to_screen("\nSquare, Cube or any power (by Prof. Pickle)\n\nSelect the number: ");
        get_string(num1_str, sizeof(num1_str));
        print_to_screen("Select the power: ");
        get_string(num2_str, sizeof(num2_str));
        num1 = atoi(num1_str);
        num2 = atoi(num2_str);
        if (num2 < 0 || num2 > 12) {
            print_to_screen("Error: Power too large or invalid.\n");
            print_to_screen("Press any key to continue...");
            get_input();
            goto main_menu;
        }
        result = 1;
        for (int i = 0; i < num2; i++) {
            result *= num1;
        }
    } else {
        print_to_screen("\nInvalid value. Press any key to try again...\n");
        get_input();
        goto main_menu;
    }

    previous_result = result;
    char result_str[16];
    itoa(result, result_str, 10);
    print_to_screen("\n------------\n");
    print_to_screen(result_str);
    print_to_screen("\n\nPress any key to return to menu...");
    get_input();
    goto main_menu;
}
