//
// Created by skushneryuk on 07/10/2020.
//

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

const uint32_t ONE_BYTE_MASK = 0;
const uint32_t ONE_BYTE_SHIFT = 7;
const uint32_t TWO_BYTE_MASK = 6;
const uint32_t TWO_BYTE_SHIFT = 5;
const uint32_t THREE_BYTE_MASK = 14;
const uint32_t THREE_BYTE_SHIFT = 4;
const uint32_t FOUR_BYTE_MASK = 30;
const uint32_t FOUR_BYTE_SHIFT = 3;
const uint32_t STANDARD_MASK = 2;
const uint32_t STANDARD_SHIFT = 6;


int main() {
    uint32_t ch;
    uint32_t convertible = 0;
    uint32_t non_convertible = 0;
    uint32_t bytes_number = 0;

    while((ch = getchar()) != EOF){
        if(ch >> ONE_BYTE_SHIFT == ONE_BYTE_MASK){
            ++convertible;
        } else {
            if(ch >> TWO_BYTE_SHIFT == TWO_BYTE_MASK) {
                bytes_number = 2;
            } else if(ch >> TWO_BYTE_SHIFT == TWO_BYTE_MASK) {
                bytes_number = 3;
            } else if(ch >> THREE_BYTE_SHIFT == THREE_BYTE_MASK) {
                bytes_number = 3;
            } else if(ch >> FOUR_BYTE_SHIFT == FOUR_BYTE_MASK) {
                bytes_number = 4;
            } else {
                printf("%"PRIu32 " %"PRIu32"\n", convertible, non_convertible);
                return 1;
            }
            for(size_t i = 1; i < bytes_number; ++i){
                ch = getchar();
                if(ch >> STANDARD_SHIFT != STANDARD_MASK){
                    printf("%"PRIu32 " %"PRIu32"\n", convertible, non_convertible);
                    return 1;
                }
            }
            ++non_convertible;
        }
    }

    printf("%"PRIu32 " %"PRIu32"\n", convertible, non_convertible);
    return 0;
}