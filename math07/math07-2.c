#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>

const long long BLOCK_SIZE = 1L << 18;

typedef int f_type;  // type of data in file

long long
min(long long a, long long b){
    return a < b ? a : b;
}

typedef struct {
    f_type* buffer;
    int ind;
    int fd;
    long long stop;
    long long curr;
} SEG_HOLDER;


typedef struct {
    f_type* buffer;
    int fd;
    int ind;
} OUT_HOLDER;


typedef struct {
    long long file_size;
    OUT_HOLDER file;
    SEG_HOLDER lseg;
    SEG_HOLDER rseg;
} FILE_HOLDER;


// output_holder methods
void
flush(OUT_HOLDER *holder) {
    write(holder->fd, holder->buffer, holder->ind * sizeof(f_type));
}

void
append(OUT_HOLDER *holder, f_type value) {
    holder->buffer[holder->ind] = value;
    ++(holder->ind);
    if (holder->ind == BLOCK_SIZE) {
        flush(holder);
        holder->ind = 0;
    }
}

void
reset_out(OUT_HOLDER *holder) {
    holder->ind = 0;
}


// segment_holder methods
void
load(SEG_HOLDER *holder){
    read(holder->fd, holder->buffer,
         sizeof(f_type) * min(BLOCK_SIZE, (holder->stop) - (holder->curr)));
}

void
inc(SEG_HOLDER *holder) {
    ++(holder->ind);
    ++(holder->curr);
    if (holder->ind == BLOCK_SIZE){
        holder->ind = 0;
        load(holder);
    }
}

f_type
get(SEG_HOLDER *holder) {
    return (holder->buffer)[holder->ind];
}

int
ended(SEG_HOLDER *holder) {
    return (holder->stop) == (holder->curr);
}

void
reset_seg(SEG_HOLDER *holder, long long size) {
    holder->ind = 0;
    holder->curr = 0;
    holder->stop = size;
    load(holder);
}


int comp_f_type(const void *a, const void *b) {
    if (*(const f_type *) a < *(const f_type *) b) return -1;
    if (*(const f_type *) a > *(const f_type *) b) return 1;
    return 0;
}


void
sort_n_load_blocks(int fd, f_type* buffer) {
    lseek(fd, 0L, SEEK_SET);
    int curr_read = read(fd, buffer, sizeof(f_type) * BLOCK_SIZE);
    while (curr_read != 0) {
        qsort(buffer, curr_read / sizeof(f_type), sizeof(f_type), comp_f_type);
        lseek(fd, -curr_read, SEEK_CUR);
        write(fd, buffer, curr_read);
        curr_read = read(fd, buffer, sizeof(f_type) * BLOCK_SIZE);
    }
}


void
copy_file_to_file(int fd_from, int fd_to, long long start_from,
                  long long start_to, long long size, f_type* buffer) {
    lseek(fd_from, start_from, SEEK_SET);
    lseek(fd_to, start_to, SEEK_SET);
    int to_copy = size;
    while (to_copy) {
        read(fd_from, buffer, sizeof(f_type) * min(BLOCK_SIZE, to_copy));
        write(fd_to, buffer, sizeof(f_type) * min(BLOCK_SIZE, to_copy));
        to_copy -= min(BLOCK_SIZE, to_copy);
    }
}

void
merge(FILE_HOLDER *holder, long long left, long long size) {
    copy_file_to_file((holder->file).fd, (holder->lseg).fd,
                      left * sizeof(f_type), 0, size,
                      holder->file.buffer);

    copy_file_to_file((holder->file).fd, (holder->rseg).fd,
                      (left + size) * sizeof(f_type), 0,
                      min(size, holder->file_size - size - left),
                      holder->file.buffer);

    lseek((holder->file).fd, left * sizeof(f_type), SEEK_SET);
    lseek((holder->lseg).fd, 0L, SEEK_SET);
    lseek((holder->rseg).fd, 0L, SEEK_SET);

    reset_out(&(holder->file));
    reset_seg(&(holder->lseg), size);
    reset_seg(&(holder->rseg), min(size, holder->file_size - size - left));

    f_type l, r;
    while(!ended(&(holder->lseg)) && !ended(&(holder->rseg))){
        l = get(&(holder->lseg));
        r = get(&(holder->rseg));

        if(l < r) {
            append(&(holder->file), l);
            inc(&(holder->lseg));
        } else {
            append(&(holder->file), r);
            inc(&(holder->rseg));
        }
    }
    while (!ended(&(holder->lseg))) {
        append(&(holder->file), get(&(holder->lseg)));
        inc(&(holder->lseg));
    }
    while (!ended(&(holder->rseg))) {
        append(&(holder->file), get(&(holder->rseg)));
        inc(&(holder->rseg));
    }
    flush(&(holder->file));
}


int main(int argc, char *argv[]) {
    int flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    // appropriate flags

    FILE_HOLDER holder;
    holder.file.buffer = (f_type *) calloc(BLOCK_SIZE, sizeof(f_type));
    holder.lseg.buffer = (f_type *) calloc(BLOCK_SIZE, sizeof(f_type));
    holder.rseg.buffer = (f_type *) calloc(BLOCK_SIZE, sizeof(f_type));

    holder.file.fd = open(argv[1], O_RDWR, flags);  // input file
    holder.file_size = lseek(holder.file.fd, 0L, SEEK_END) / sizeof(f_type);
    // number of elements

    char template1[] = "tmp_file1XXXXXX";
    char template2[] = "tmp_file2XXXXXX";
    holder.lseg.fd = mkstemp(template1);  // temporary files
    holder.rseg.fd = mkstemp(template2);  //
    fchmod(holder.lseg.fd, flags);  // changing access rights
    fchmod(holder.rseg.fd, flags);  // changing access rights

    sort_n_load_blocks(holder.file.fd, holder.file.buffer);


    long long seg_size = BLOCK_SIZE;
    long long left;
    while (seg_size < holder.file_size){
        left = 0;
        while (left + seg_size < holder.file_size){
            merge(&holder, left, seg_size);
            left += 2 * seg_size;
        }
        seg_size <<= 1;
    }

    end_program:
    free(holder.file.buffer);
    free(holder.lseg.buffer);
    free(holder.rseg.buffer);
    close(holder.file.fd);
    close(holder.rseg.fd);
    close(holder.lseg.fd);
    remove(template1);
    remove(template2);

    return 0;
}