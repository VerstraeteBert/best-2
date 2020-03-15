#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

const int SIZE = 10000000;
const int THREAD_POOL = 4;

void fill_backwards(int [], size_t);
void* merge_sort_new_thread(void* v_m_data);
void merge_sort_parallel(int [], size_t);
void merge_sort_parallel_helper(int arr[], int start, int end, int* thread_pool);
void merge(int [], int start, int mid, int end);
void merge_sort_seq(int[], int n);
void merge_sort_seq_helper(int [], int start, int end);

struct merge_data {
    int * arr;
    int start;
    int end;
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
    int thread_pool = THREAD_POOL - 1;
    merge_sort_parallel_helper(arr, 0, (int) n - 1, &thread_pool);
}

void merge_sort_parallel_helper(int arr[], int start, int end, int* thread_pool) {
    if (start < end) {
        int mid = (start + end) / 2;
        if (*thread_pool >= 1) {
            (*thread_pool) -= 1;
//          spawned thread will handle first half, current thread will handle second half
            struct merge_data m_data = {
                    .arr = arr,
                    .start = start,
                    .end = mid,
                    .thread_pool = thread_pool
            };
            printf("Spawning new thread for: %d -> %d\n", start, mid);
            printf("Current thread handling: %d -> %d\n", mid + 1, end);
            pthread_t merge_thr;
            pthread_create(&merge_thr, NULL, merge_sort_new_thread, (void*) &m_data);

            merge_sort_parallel_helper(arr, mid + 1, end, thread_pool);

            pthread_join(merge_thr, NULL);
        } else {
            merge_sort_parallel_helper(arr, start, mid, thread_pool);
            merge_sort_parallel_helper(arr, mid + 1, end, thread_pool);
        }
//        printf("Para Merging %d -> %d\n", start, end);
        merge(arr, start, mid, end);
    }
}

void* merge_sort_new_thread(void* v_m_data) {
    struct merge_data* m_data = (struct merge_data*) v_m_data;
    merge_sort_parallel_helper(m_data->arr, m_data->start, m_data->end, m_data->thread_pool);
    return NULL;
}

void merge_sort_seq(int arr[], int n) {
    merge_sort_seq_helper(arr, 0, n - 1);
}

void merge_sort_seq_helper(int arr [], int start, int end) {
    if (start < end) {
        int mid = (start + end) / 2;
        merge_sort_seq_helper(arr, start, mid);
        merge_sort_seq_helper(arr, mid + 1, end);
//        printf("Seq merging: %d -> %d\n", start, end);
        merge(arr, start, mid, end);
    }
}

void merge (int arr [], int start, int mid, int end) {
    int l = start; int r = mid + 1; int curr = 0;

    if (arr[mid] <= arr[r]) {
        return;
    }

    int * temp = malloc((end - start + 1)  * sizeof(int));

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
        arr[start++] = temp[i];
    }
    free(temp);
}
