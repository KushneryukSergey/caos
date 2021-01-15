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

int main(int argc, char* argv[]) {
    char name[PATH_MAX + 1];
    char* check;
    f_info info;
    unsigned long long answer = 0;
    
    while (fgets(name, PATH_MAX + 1, stdin)) {
        // fgets often return string with newline at the end
        check = memchr(name, '\n', PATH_MAX + 1);
        if (check != NULL)
            *check = '\0';
    
        lstat(name, &info);
        if (S_ISREG(info.st_mode))
            answer += info.st_size;
    }
    
    printf("%llu\n", answer);
    return 0;
}