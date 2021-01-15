#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>

struct Item {
    int value;
    uint32_t next_pointer;
};

typedef struct Item Item;
typedef struct stat f_info;

int main(int argc, char* argv[]) {
    const char* filename = argv[1];
    int fd = open(filename, O_RDONLY);
    f_info info;
    stat(filename, &info);
    
    if (!info.st_size)
        return 0;
    
    void* file = mmap(NULL,
                      info.st_size,
                      PROT_READ,
                      MAP_PRIVATE,
                      fd,
                      0);
    close(fd);
    
    Item* elem = file;
    
    while (elem->next_pointer) {
        printf("%d\n", elem->value);
        elem = file + elem->next_pointer;
    }
    printf("%d\n", elem->value);
    
    munmap(file, info.st_size);
    return 0;
}

