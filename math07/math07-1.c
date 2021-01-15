#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>


struct Item {
    int value;
    uint32_t next_pointer;
};

typedef struct Item Item;

int main(int argc, char *argv[]) {
    Item item;
    int bin = open(argv[1], O_RDONLY | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);

    int curr_read = read(bin, &item, sizeof(Item));

    if (!curr_read) {
        close(bin);
        return 0;
    }

    while (item.next_pointer) {
        printf("%d\n", item.value);
        lseek(bin, item.next_pointer, SEEK_SET);
        read(bin, &item, sizeof(Item));
    }
    printf("%d\n", item.value);

    close(bin);
}