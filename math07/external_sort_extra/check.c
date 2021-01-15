#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>

#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    // appropriate flags
    int out = open(argv[1], O_RDWR, flags);  // input file

    int buf[SIZE];
    int curr_read = read(out, buf, sizeof(buf));
    int i = 0;
    //printf("Curr_read: %d\n", curr_read);
    while(curr_read) {
        if(i == BLOCK / 2){
            for (int j = 0; j < curr_read / sizeof(int); ++j) {
                printf("%d\n", buf[j]);
            }
        }
        ++i;
        curr_read = read(out, buf, sizeof(buf));
        //printf("Curr_read: %d\n", curr_read);
    }
    close(out);
    return 0;
}