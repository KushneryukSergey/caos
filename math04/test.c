#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void merge(int from, int mid, int to, int* out);

void mergesort_helper(int from, int to, int* out);

void mergesort(int from, int to, const int *in, int* out);
/*{
    for(int i = from; i < to; ++i){
        *(out + i - from) = *(in + i);
    }
    printf("this is not a problem\n");
    mergesort_helper(0, to - from, out);
}*/


int main(){
    int a[] = {19, 3, 15, 16, 1, 0, 2, 5, 4, 40};
    int* ap = a;
    int t[] = {19, 3, 15, 16, 1, 0, 2, 5, 4, 40};
    int clear[] = {0, 0, 0, 0, 0, 0};
    int r[2] = {0, 0};
    int *test = t;
    int* res = r;
    //merge(2, 3, 4, test);
    for(int i = 0; i <= 4; ++i){
        for (int j = i + 2; j <= i + 2; ++j) {
            for (int i = 0; i < 6; ++i) {
                *(res + i) = *(clear + i);
            }
            mergesort(i, j, test, res);
            printf("%d %d\ntest: ", i, j);
            for (int k = 0; k < 6; ++k) {
                printf("%d ", *(test + k));
            }
            printf("\n\n res: ");
            for (int k = 0; k < 2; ++k) {
                printf("%d ", *(res + k));
            }
            printf("\n_______________\n");
        }
    }

}
