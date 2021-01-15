//
// Created by skushneryuk on 06/10/2020.
//

#include <stdio.h>
#include <stdint.h>

uint64_t get_bit_on_pos(const uint64_t* mask, const uint16_t* shift){
    return (*mask >> *shift) & (uint64_t)1;
}

void set_bit_on_pos(uint64_t* mask, const uint16_t shift){
    *mask |= (uint64_t)1 << shift;
}

void add_to_set(uint64_t *set, const int* element){
    if(*element >= 48 && *element < 58){
        set_bit_on_pos(set, *element - '0');
    } else if(*element >= 65 && *element < 91){
        set_bit_on_pos(set, *element - 'A' + 10);
    } else if(*element >= 97 && *element < 123){
        set_bit_on_pos(set, *element - 'a' + 36);
    }
}

char get_set_element(const uint16_t* shift){
    if(*shift < 10){
        return (char)(48 + *shift);
    } else if(*shift < 36){
        return (char)(65 + *shift - 10);
    } else {
        return (char)(97 + *shift - 36);
    }
}

int is_operation(const int* element){
    return *element == '|' || *element == '&' ||
        *element == '^' || *element == '~';
}

void operate_set(uint64_t *set1, const uint64_t *set2, const int* operation){
    if(*operation == '|'){
        *set1 |= *set2;
    }
    if(*operation == '^'){
        *set1 ^= *set2;
    }
    if(*operation == '&'){
        *set1 &= *set2;
    }
    if(*operation == '~'){
        *set1 = ~(*set1);
    }
}

void print_set(const uint64_t *set){
    for(uint16_t i = 0; i < 62; ++i){
        if(get_bit_on_pos(set, &i)){
            printf("%c", get_set_element(&i));
        }
    }
}

int main() {
    uint64_t current = 0;
    uint64_t next = 0;
    int ch = ' ';
    while((ch = getchar()) != EOF) {
        if (is_operation(&ch)) {
            operate_set(&current, &next, &ch);
            next = 0;
        } else {
            add_to_set(&next, &ch);
        }
    }
    print_set(&current);
    printf("\n");
}