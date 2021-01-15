#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>

const int BUFFER_CP = 4096;

int read_from_file(int fd, char* buf) {
    return read(fd, buf, BUFFER_CP);
}

typedef struct {
    int fd;
    char buf[4096];
    int sz;
} OUT_FILE;


int flush(OUT_FILE *file) {
    int copy_sz = file->sz;
    file->sz = 0;
    return write(file->fd, file->buf, copy_sz);
}

int append(OUT_FILE* file, char ch) {
    file->buf[file->sz] = ch;
    ++(file->sz);
    if (file->sz == BUFFER_CP) {
        return flush(file);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 4)
        return 3;
    
    int input_fd = open(argv[1], O_RDONLY);
    if (input_fd < 0) {
        close(input_fd);
        return 1;
    }

    OUT_FILE digit_out, else_out;
    digit_out.fd = open(argv[2], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    digit_out.sz = 0;
    if (digit_out.fd < 0) {
        close(input_fd);
        close(digit_out.fd);
        return 2;
    }
    
    char input[BUFFER_CP];
    int ret = 0;
    else_out.fd = open(argv[3], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    else_out.sz = 0;
    if (else_out.fd < 0) {
        ret = 2;
        goto end;
    }

    int curr_read;
    while ((curr_read = read_from_file(input_fd, input))) {
        if (curr_read < 0) {
            ret = 3;
            goto end;
        }

        for (int pos = 0; pos < curr_read; ++pos) {
            if (isdigit(input[pos])) {
                ret = append(&digit_out, input[pos]);
            } else {
                ret = append(&else_out, input[pos]);
            }
            if (ret < 0) {
                ret = 3;
                goto end;
            }
        }
    }

    ret = flush(&digit_out);
    if (ret < 0) {
        ret = 3;
        goto end;
    }
    ret = flush(&else_out);
    if (ret < 0) {
        ret = 3;
        goto end;
    }
    ret = 0;
    
    end:
    close(input_fd);
    close(digit_out.fd);
    close(else_out.fd);
    return ret;
}