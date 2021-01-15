#include <stdio.h>
#include <stdlib.h>
#include <immintrin.h>

int main() {
    int N, M;
    scanf("%d %d", &N, &M);
    int AM = M; // aligned N
    if(AM % 8){
        AM = ((AM >> 3) + 1) << 3;
    }
    float* A = aligned_alloc(256, AM * N * sizeof *A);
    float* B = aligned_alloc(256, AM * N * sizeof *B);
    for(int i = 1; i <= N; ++i){
        for (int j = 1; j <= 8; ++j) {
            A[i * AM - j] = 0;
            B[i * AM - j] = 0;
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            scanf("%f", (A + i * AM + j));
        }
    }
    for (int j = 0; j < M; ++j) {
        for (int i = 0; i < N; ++i) {
            scanf("%f", (B + i * AM + j));
        }
    }

    __m256 save;
    float *R = malloc(N * N * sizeof *B);
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            R[i * N + j] = 0;
            for (int k = 0; k < AM; k += 8) {
                save = _mm256_dp_ps(*(__m256 * )(A + i * AM + k), *(__m256 * )(B + j * AM + k), 0xF1);
                R[i * N + j] += save[0] + save[4];
            }
        }
    }

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            printf("%.6f ", R[i * N + j]);
        }
        printf("\n");
    }
}