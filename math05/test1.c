#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <xmmintrin.h>


float test(const float *A, const float *B){
    return *A * *B;
}


float dot_product1(float *restrict a_, float *restrict b_) {
    float answer = 0;
    float *a = __builtin_assume_aligned(a_, 32);
    float *b = __builtin_assume_aligned(b_, 32);
    for (int i = 0; i < 8; ++i)
        answer += a[i]*b[i];
    return answer;
}