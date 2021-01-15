#include <sys/syscall.h>

long
syscall(long number, ...);

void
my_exit(int code) {
    syscall(SYS_exit, code);
}

int
my_write(int file_desc, const char *buf, int size) {
    return syscall(SYS_write, file_desc, buf, size);
}

void
_start() {
    char *str = "Hello, World!";
    int len = 13;
    my_write(1, str, len);
    my_exit(0);
}