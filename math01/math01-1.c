//
// Created by skushneryuk on 06/10/2020.
//

#include <stdio.h>
#include <stdint.h>

typedef enum {
    PlusZero      = 0x00,
    MinusZero     = 0x01,
    PlusInf       = 0xF0,
    MinusInf      = 0xF1,
    PlusRegular   = 0x10,
    MinusRegular  = 0x11,
    PlusDenormal  = 0x20,
    MinusDenormal = 0x21,
    SignalingNaN  = 0x30,
    QuietNaN      = 0x31
} float_class_t;

typedef union {
    uint64_t uint_value;
    double real_value;
} double_or_uint;

const size_t EXPONENT_SIZE = 11;
const size_t MANTISSA_SIZE = 52;

uint64_t get_bit_on_pos(const uint64_t* value, const size_t shift){
    return (*value >> shift) & (uint64_t)1;
}

int get_sign(const uint64_t* value){
    return *value >> (EXPONENT_SIZE + MANTISSA_SIZE);
}

uint64_t get_exponent(const uint64_t* value){
    return *value >> MANTISSA_SIZE & (((uint64_t)1 << EXPONENT_SIZE) - 1);
}

uint64_t get_mantissa(const uint64_t* value){
    return *value & (((uint64_t)1 << MANTISSA_SIZE) - 1);
}

extern float_class_t
classify(double *value_ptr) {
    double_or_uint helper;
    helper.real_value = *value_ptr;
    uint64_t bit_presentation = helper.uint_value;
    int sign = get_sign(&bit_presentation);
    uint64_t exponent = get_exponent(&bit_presentation);
    uint64_t mantissa = get_mantissa(&bit_presentation);
    if(exponent == 0){
        if(mantissa == 0){
            if(sign){
                return MinusZero;
            } else {
                return PlusZero;
            }
        } else {
            if(sign){
                return MinusDenormal;
            } else {
                return PlusDenormal;
            }
        }
    } else if(exponent == ((uint64_t)1<<EXPONENT_SIZE) - 1){
        if(mantissa == 0){
            if(sign){
                return MinusInf;
            } else {
                return PlusInf;
            }
        } else {
            if(get_bit_on_pos(&mantissa, MANTISSA_SIZE - 1)){
                return QuietNaN;
            } else {
                return SignalingNaN;
            }
        }
    } else {
        if(sign){
            return MinusRegular;
        } else {
            return PlusRegular;
        }
    }
}