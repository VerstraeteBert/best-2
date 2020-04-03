#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

const int DIM = 1000;
const int THREAD_POOL = 4; // aantal hardware threads (cores)

/**
 * opnieuw maken
 *  -> argumenten matrix_a, matrix_b , matrix_res en offset meegeven in struct ip global
 */

int matrix_src [DIM][DIM];
int matrix_res [DIM][DIM];

void* mult_matrix(void* v_offset) {
    int* offset = (int*) v_offset;
    size_t i; size_t j; size_t k;
    for (i = *offset; i < DIM; i += THREAD_POOL) {
        for (j = 0; j < DIM; j++) {
            matrix_res[i][j] = 0;
            for (k = 0; k < DIM; k++) {
                matrix_res[i][j] += matrix_src[i][k] * matrix_src[k][j];
            }
        }
    }
    free(offset);
    return NULL;
}

int main(void) {
    int i; int j;
    for (i = 0; i < DIM; i++) {
        for (j = 0; j < DIM; j++) {
            matrix_src[i][j] = i + j;
        }
    }

    pthread_t threads [THREAD_POOL];
    for(i = 0; i < THREAD_POOL; i++) {
        int* offset = malloc(sizeof(int));
        *offset = i;
        pthread_create(&threads[i], NULL, mult_matrix, (void*) offset);
    }
    // opnieuw optimilisatie mogelijk door één de berekeningen niet te laten uitvoeren door een extra gespawnde thread
    // de main thread is opnieuw maar aan het busy waiten
    // exercise for the reader enzo
    for (i = 0; i < THREAD_POOL; i++) {
        pthread_join(threads[i], NULL);
    }

//    for (i = 0; i < DIM; i++) {
//        for (j = 0; j < DIM; j++) {
//            printf("%d ", matrix_res[i][j]);
//        }
//        printf("\n");
//    }

    return 0;
}
