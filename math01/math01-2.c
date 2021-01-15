//
// Created by skushneryuk on 06/10/2020.
//

#include <stdint.h>

uint16_t satsum(uint16_t x, uint16_t y){
    uint16_t result = x + y;
    if(result >= x && result >= y){
        return result;
    } else {
        return (uint16_t)(-1);
    }
}