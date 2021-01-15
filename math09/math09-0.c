#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct stat f_info;

int main(int argc, char* argv[]) {
    const char* filename = argv[1];
    const char* tmp = argv[2];
    
    int fd = open(filename, O_RDONLY);
    f_info info;
    stat(filename, &info);
    size_t tmp_len = strlen(tmp);
    
    char* copy_file = mmap(NULL,
                           info.st_size,
                           PROT_READ,
                           MAP_PRIVATE,
                           fd,
                           0);
    close(fd);
    
    char* end = copy_file + info.st_size;
    for (char* p = copy_file;
         (p = memmem(p, end - p, tmp, tmp_len)) != NULL;
         ) {
        printf("%ld\n", p++ - copy_file);
    }
    
    munmap(copy_file, info.st_size);
    return 0;
}