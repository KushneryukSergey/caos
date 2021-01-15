#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern float
dot_product(int N, const float *A, const float *B);

int main() {
    for(int i = 0; i < 2048; ++i){
        printf("%d\n", i);
    }
}
