#include <windows.h>
#include <stdio.h>
#include <inttypes.h>


struct Item {
    int value;
    uint32_t next_pointer;
};

typedef struct Item Item;

int main(int argc, char *argv[]) {
    Item item[1];

    HANDLE bin = CreateFileA(argv[1],
                             GENERIC_READ,
                             FILE_SHARE_READ,
                             NULL,
                             OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL,
                             NULL);
    // non-Win analog open(...)

    DWORD curr_read;
    BOOL success;
    success = ReadFile(bin, item, sizeof(item), &curr_read, NULL);
    // non-Win analog read(...)

    if(curr_read) {
        while (item[0].next_pointer) {
            printf("%d\n", item[0].value);
            SetFilePointer(bin, item[0].next_pointer, NULL, FILE_BEGIN);
            // non-Win analog lseek

            success = ReadFile(bin, item, sizeof(item), &curr_read, NULL);
        }
        printf("%d\n", item[0].value);
    }

    CloseHandle(bin);
    //  non-Win analog close(...);
}