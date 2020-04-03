#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

const int DIM = 1000;

void mult_matrix(const int src[DIM][DIM], int res[DIM][DIM]) {
    int accumulator; size_t i; size_t j; size_t k;
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            res[i][j] = 0;
            for (k = 0; k < DIM; k++) {
                res[i][j] += src[i][k] * src[k][j];
            }
        }
    }
}

int main(void) {
    int matrix_src [DIM][DIM];
    int matrix_res [DIM][DIM];
    int i; int j;
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            matrix_src[i][j] = i + j;
        }
    }

    mult_matrix(matrix_src, matrix_res);

    return 0;
}
