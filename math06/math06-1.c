#include <sys/syscall.h>

const int STDIN = 0;
const int STDOUT = 1;
const int STDERR = 2;

long
syscall(long number, ...);

void
my_exit(int code) {
    syscall(SYS_exit, code);
}

int
my_read(int file_desc, const char *buf, int size) {
    return syscall(SYS_read, file_desc, buf, size);
}

int
my_write(int file_desc, const char* buf, int size) {
    return syscall(SYS_write, file_desc, buf, size);
}

int
my_read_str(int file_desc, const char *buf, int to_read) {
    int curr_read;  // currently read
    int left_to_read = to_read;
    while (left_to_read &&
           (curr_read = my_read(file_desc,
                                buf + to_read - left_to_read,
                                left_to_read))) {
        left_to_read -= curr_read;
    }
    return to_read - left_to_read;
}

int
my_write_str(int file_desc, const char *buf, int to_write) {
    int curr_write;  // currently write
    int left_to_write = to_write;
    while (left_to_write &&
           (curr_write = my_write(file_desc,
                                  buf + to_write - left_to_write,
                                  left_to_write))) {
        left_to_write -= curr_write;
    }
    return to_write - left_to_write;
}

void
_start() {
    const int buf_size = 1 << 12; // 4096 bytes
    char buf[buf_size];
    int curr_read;  // currently read
    while ((curr_read = my_read_str(STDIN, buf, buf_size))) {
        //my_write(STDERR, "a\n", 2);
        my_write_str(STDOUT, buf, curr_read);
    }
    my_exit(0);
}