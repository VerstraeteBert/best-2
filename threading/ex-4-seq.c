#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const int SIZE = 10000000;

void fill_backwards(int [], size_t);
void merge_sort(int [], size_t);
void merge(int [], size_t);

int main(void) {
    int* arr = malloc(SIZE * sizeof(int));
    int* arr2 = malloc(SIZE * sizeof(int));
    fill_backwards(arr, SIZE);
    memcpy(arr2, arr, SIZE * sizeof(int));
    merge_sort(arr, SIZE);
    merge_sort(arr2, SIZE);
    free(arr);
    free(arr2);
}

void fill_backwards(int arr [], size_t n) {
    int i;
    for (i = 0; i < n; i++) {
        arr[i] = n - i;
    }
}

void merge_sort(int arr [], size_t n) {
    if (n > 1) {
        merge_sort(arr, n/2);
        merge_sort(arr + n/2, n - n/2);
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
