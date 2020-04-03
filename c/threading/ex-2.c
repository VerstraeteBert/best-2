#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 1000000
#define RAN_MAX 2000000

void* get_min(void* v_arr) {
	double* arr = (double*) v_arr;
    double min = arr[0];
	size_t i;
	for (i = 1; i < N; i++) {
		if (arr[i] < min) {
			min = arr[i];
		}
	}
    printf("Min is: %f\n", min);
	return NULL;
}

void* get_max(void* v_arr) {
    double * arr = (double*) v_arr;
	double max = arr[0];
	size_t i;
	for (i = 1; i < N; i++) {
		if (arr[i] > max) {
			max = arr[i];
		}
	}
    printf("Max is: %f\n", max);
	return NULL;
}

int main(void) {
	srand(getpid());
	double arr [N];
	int i;
	for (i = 0; i < N; i++) {
		arr[i] = RAN_MAX * (rand() / (double) RAND_MAX);
	}

	pthread_t min_t; pthread_t max_t;
	pthread_create(&min_t, NULL, get_min, (void*) arr);
    pthread_create(&max_t, NULL, get_max, (void*) arr);
	pthread_join(min_t, NULL);
	pthread_join(max_t, NULL);

	return 0;
}
