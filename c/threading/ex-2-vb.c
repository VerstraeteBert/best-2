#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int* array;
    int n;
    int (*f)(int*, int);
    int resultaat;
} argument;

int kleinste(int * array, int n) {
    int kleinste = array[0];
    int i;
    for(i = 1; i < n; i++) {
        if (array[i] < kleinste) kleinste = array[i];
    }
    return kleinste;
}

int grootste(int * array, int n) {
    int grootste = array[0];
    int i;
    for(i = 1; i < n; i++) {
        if (array[i] > grootste) grootste = array[i];
    }
    return grootste;
}

void* worker(void* a) {
    argument* arg = (argument*) a;
    arg->res = (arg->f(arg->array, arg->n));
    return NULL;
}

int main() {
    int arr [5] = {1,7,-15,8,5};
    pthread_t thr1, thr2;
    argument a, b;
    a.array = arr;
    b.array = arr;
    a.n = 5;
    b.n = 5;
    a.f = kleinste;
    b.f = grootste;

    pthread_create(&thr1, NULL, worker, (void*) &a);
    pthread_create(&thr2, NULL, worker, (void*) &b);

    pthread_join(thr1, NULL);
    printf("Kleinste: %d\n", a.res);
    pthread_join(thr2, NULL);
    printf("Grootste: %d\n", b.res);

    return 0;
}




