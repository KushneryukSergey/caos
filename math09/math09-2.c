#define _GNU_SOURCE

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>

void next(int* i, int* j, int N) {
    if (*i == N / 2 && *j == (N - 1) / 2) {
        *i = -1;
        *j = -1;
        return;
    } else if (*j >= *i - 1 && *j < N - 1 - *i) {
        ++(*j);
        return;
    } else if (*j <= *i && *j > N - 1 - *i) {
        --(*j);
        return;
    } else if (*j > *i && *i >= N - 1 - *j) {
        ++(*i);
    } else {
        --(*i);
    }
}

void write_int(char* file, int W, int value) {
    snprintf(file, W + 1,"%*d", W, value);
    file[W] = ' ';
}

void fill_file(char* file, int N, int W) {
    int i = 0;
    int j = 0;
    int value = 1;
    int pos;
    while (i != -1 && j != -1) {
        pos = (N * W + 1) * i + j * W;
        write_int(file + pos, W, value++);
        next(&i, &j, N);
    }
    pos = N * W;
    for(i = 0; i < N; ++i) {
        file[pos] = '\n';
        pos += (N * W + 1);
    }
}

int main(int argc, char* argv[]) {
    const char* filename = argv[1];
    int N = atoi(argv[2]);
    int W = atoi(argv[3]);
    
    int file_size = (W * N + 1) * N;
    remove(filename);
    int flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    int fd = open(filename, O_RDWR | O_CREAT, flags);
    
    fallocate(fd, 0, 0, file_size);
    char* file = mmap(NULL,
                      file_size,
                      PROT_WRITE | PROT_READ,
                      MAP_SHARED,
                      fd,
                      0);
    close(fd);
    
    fill_file(file, N, W);
    munmap(file, file_size);
    
    return 0;
}