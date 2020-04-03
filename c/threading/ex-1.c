#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4
#define N2 1000

void* worker(void *a) {
	int* n = (int*) a;
	int i;
	for (i = 0; i < N2; i++) {
		printf("%d", *n);
	}
	return NULL;
}

int main(int argc, char **argv) {
	srand(time(0));
	pthread_t threads[N];

	int numbers[N] = {1, 2, 3, 4};

	int i;
	for (i = 0; i < N; i++) {
		pthread_create(&threads[i], NULL, worker, (void*) &numbers[i]);
	}
	for (i=0; i < N; i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
