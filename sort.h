
#include <stdio.h>
#include <stdlib.h>

// Function to swap two elements in an array
void swap(__attribute__((address_space(255))) int * a, __attribute__((address_space(255))) int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Function to partition the array and return the pivot index
int partition(__attribute__((address_space(255))) int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i+1], &arr[high]);
    return i+1;
}

// Function to implement Quick Sort
void quickSort(__attribute__((address_space(255))) int *arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);

        // Recursively sort the left and right partitions
        quickSort(arr, low, pivot-1);
        quickSort(arr, pivot+1, high);
    }
}




void bubble_sort(__attribute__((address_space(255))) int *arr, int l, int r){
    int temp = 0;
    for(int i = l; i<r-1;i++){
        for(int j = l; j<r-1; j++){
            if(arr[j] > arr[j+1]){
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}