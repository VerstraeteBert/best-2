#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

const int SIZE = 10000000;
const int THREAD_POOL = 4;

void fill_backwards(int [], size_t);
void* merge_sort_new_thread(void* v_m_data);
void merge_sort_parallel(int [], size_t);
void merge_sort_parallel_helper(int arr[], size_t n, int* thread_pool);
void merge(int [], size_t);
void merge_sort_seq(int [], size_t);

struct merge_data {
    int * arr;
    size_t n;
    int* thread_pool;
} merge_data;

int main(void) {
    int* arr = malloc(SIZE * sizeof(int));
    int* arr2 = malloc(SIZE * sizeof(int));
    fill_backwards(arr, SIZE);
    fill_backwards(arr2, SIZE);

    double start = clock();
    merge_sort_seq(arr, SIZE);
    double time = (clock() - start) / CLOCKS_PER_SEC;
    printf("SEQUENTIAL: TIME=%.2f\n", time);

    start = clock();
    merge_sort_parallel(arr2, SIZE);
    time = (clock() - start) / CLOCKS_PER_SEC;
    printf("PARALLEL: TIME=%.2f\n", time);

    free(arr);
    free(arr2);
}

void fill_backwards(int arr [], size_t n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void merge_sort_parallel(int arr [], size_t n) {
    int* thread_pool = malloc(sizeof(int));
    *thread_pool = THREAD_POOL;
    merge_sort_parallel_helper(arr, n, thread_pool);
    free(thread_pool);
}

void merge_sort_parallel_helper(int arr[], size_t n, int* thread_pool) {
    if (n > 1) {
        if (*thread_pool >= 1) {
            // spawned thread will handle first half, current thread will handle second half
            (*thread_pool) -= 1;
            struct merge_data* m_data = (struct merge_data*) malloc(sizeof(merge_data));
            m_data->thread_pool = thread_pool;
            m_data->n = n/2;
            m_data->arr = arr;
            pthread_t merge_thr;
            pthread_create(&merge_thr, NULL, merge_sort_new_thread, (void*) m_data);

            merge_sort_parallel_helper(arr + n/2, n-n/2, thread_pool);

            pthread_join(merge_thr, NULL);
            free(m_data);
        } else {
            merge_sort_parallel_helper(arr, n/2, thread_pool);
            merge_sort_parallel_helper(arr + n/2, n - n/2, thread_pool);
        }
        merge(arr, n);
    }
}

void* merge_sort_new_thread(void* v_m_data) {
    struct merge_data* m_data = (struct merge_data*) v_m_data;
    merge_sort_parallel_helper(m_data->arr, m_data->n, m_data->thread_pool);
    return NULL;
}

void merge_sort_seq(int arr [], size_t n) {
    if (n > 1) {
        merge_sort_seq(arr, n/2);
        merge_sort_seq(arr + n/2, n - n/2);
        merge(arr, n);
    }
}

void merge (int arr [], size_t n) {
    size_t l = 0;
    size_t r = (n/2);
    size_t end = n - 1;
    size_t mid = n/2 - 1;
    size_t curr = 0;

    if (arr[mid] <= arr[r]) {
        return;
    }

    int * temp = malloc(n * sizeof(int));

    while (l <= mid && r <= end) {
        if (arr[l] <= arr[r]) {
            temp[curr] = arr[l];
            l++;
        } else {
            temp[curr] = arr[r];
            r++;
        }
        curr++;
    }

    while (l <= mid) {
        temp[curr] = arr[l];
        curr++;
        l++;
    }

    int i;
    for (i = 0; i < curr; i++) {
        *arr = temp[i];
        arr++;
    }
    free(temp);
}
