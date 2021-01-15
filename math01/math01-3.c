//
// Created by skushneryuk on 06/10/2020.
//

#include <stdio.h>

int get_size(const char* str){
    size_t size = 0;
    size_t shift = 2;
    while(str[size + shift] != '\0'){
        if(str[size + shift] == '0' && !size){
            ++shift;
        } else {
            ++size;
        }
    }
    if(size){
        return size;
    } else {
        return 1;
    }
}

int main(int argc, char* argv[]) {
    for(int i = 1; i < argc; ++i){
        size_t size = get_size(argv[i]);
        printf("%lu ", (size + 1) / 2);
    }
    print_set()
}