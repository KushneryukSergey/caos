#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

extern float
dot_product(int N, const float *A, const float *B);

int main() {
    int n = 10;
    float* a = malloc(n * sizeof *a);
    float* b = malloc(n * sizeof *b);
    for(int i = 0; i < n; ++i){
        a[i] = (i + 1);
        b[i] = (i + 1);
    }
    for(int i = n; i <= n; ++i){
        printf("%d %.10f\n", i, dot_product(i, a, b));
    }
    free(a);
    free(b);
}
