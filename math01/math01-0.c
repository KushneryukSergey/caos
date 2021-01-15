//
// Created by skushneryuk on 06/10/2020.
//

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    float x;
    int y;
    int z = strtol(argv[1], NULL, 27);
    scanf("%f %x", &x, &y);
    float res = x + (float)y + (float)z;
    printf("%.3f\n", res);
}