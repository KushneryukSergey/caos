#include <fcntl.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct stat f_info;

int is_exec(f_info info) {
    return S_IEXEC & info.st_mode;
}

int is_ELF(int fd) {
    lseek(fd, 0, SEEK_SET);
    
    char check[4];
    ssize_t curr_read = read(fd, check, 4);
    if (curr_read == 4) {
        if (check[0] == 0x7F &&
            check[1] == 'E' &&
            check[2] == 'L' &&
            check[3] == 'F') {
            return 1;
        }
    }
    return 0;
}

int is_script(int fd) {
    lseek(fd, 0, SEEK_SET);
    
    char check[2];
    ssize_t curr_read = read(fd, check, 2);
    if (curr_read == 2) {
        if (check[0] == '#' &&
            check[1] == '!') {
            char name[PATH_MAX + 1];
            read(fd, name, PATH_MAX + 1);
            strtok(name, "\n");
            
            f_info info;
            lstat(name, &info);
            
            return is_exec(info);
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    char filename[PATH_MAX + 1];
    
    while (fgets(filename, PATH_MAX, stdin)) {
        strtok(filename, "\n");
        int fd = open(filename, O_RDONLY);
        f_info info;
        lstat(filename, &info);
        
        if (fd > 0 && is_exec(info)) {
            if (is_ELF(fd) || is_script(fd))
                continue;
            
            printf("%s\n", filename);
        }
    }
    return 0;
}