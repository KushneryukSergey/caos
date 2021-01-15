#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <ctype.h>
#include <inttypes.h>

#ifdef EXTERNAL

#include <time.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int flags = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
    // appropriate flags
    int out = open(argv[1], O_RDWR | O_CREAT, flags);  // input file

    int buf[SIZE];

    srand(time(NULL));
    for(int i = 0; i < BLOCK; ++i){
        for(int j = 0; j < sizeof(buf) / sizeof(int); ++j){
            //buf[j] = 40 - j;
            buf[j] = rand();
            if(i == BLOCK / 2){
                printf("%d\n", buf[j]);
            }
        }
        if(i == BLOCK / 2){
            printf("WTF\n");
            for(int j = 0; j < sizeof(buf) / sizeof(int); ++j){
                //buf[j] = 40 - j;
                buf[j] = rand();
                printf("%d\n", buf[j]);
            }
            printf("WTF\n");

        }
        write(out, buf, sizeof(buf));
    }
    close(out);
}
#endif


#ifdef LIST
struct Item {
    int value;
    uint32_t next_pointer;
};

typedef struct Item Item;


int main() {
    FILE *bin = fopen("input.bin", "wb");
    Item list[5];
    return 0;
    list[0].value = 1;  // 0
    list[0].next_pointer = 24;
    list[1].value = 2;  // 8
    list[1].next_pointer = 0;
    list[2].value = 3;  // 16
    list[2].next_pointer = 24;
    list[3].value = 4;  // 24
    list[3].next_pointer = 8;
    list[4].value = 5;  // 32
    list[4].next_pointer = 0;

    fwrite(list, sizeof(list), 1, bin);
    fclose(bin);
}

#endif
