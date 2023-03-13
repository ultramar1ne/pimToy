#include <mram.h>
#include <defs.h>
#include <barrier.h>

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "./support/common.h"
#define len 240
#define len_per_thread 60

#include "sort.h"

BARRIER_INIT(my_barrier, NR_TASKLETS);
BARRIER_INIT(merge_barrier, 2);

__mram_noinit int arr[len];
__mram int L[len];
__mram int R[len];

// Macro to merge two subarrays of arr[].
// First subarray is arr[l..m]
// Second subarray is arr[m+1..r]
// 0 31 63
// l = 0 m = 32  r = 64
void merge(__attribute__((address_space(255))) int *arr, int l, int m, int r, int offset)
{
    for(int i = 0; i<m-l; i++){
        L[i+offset] = arr[l+i];
    }
    for(int i = 0; i<r-m; i++){
        R[i+offset] = arr[i+m];
    }

    int i = 0, j = 0, k = l;
    while (i < m-l && j < r-m) {
        if (L[i+offset] <= R[j+offset]) {
            arr[k++] = L[offset + i++];
        } else {
            arr[k++] = R[offset + j++];
        }
    }

    while (i < m-l) {
        arr[k++] = L[offset + i++];
    }

    while (j < r-m) {
        arr[k++] = R[offset+ j++];
    }
    return;
}


int main() {
    printf("this is tasklet: %u\n",me());
    quickSort(arr,me()*len_per_thread,(me()+1)*len_per_thread-1);
    barrier_wait(&my_barrier);
    if(me()==0){
        printf("here we merge 0\n");
        merge(arr,0,len_per_thread,2*len_per_thread,me()*len_per_thread);
        barrier_wait(&merge_barrier);
        printf("here we merge 00\n");
        merge(arr,0,2*len_per_thread,len,me()*len_per_thread);
    }
    if(me()==1){
        printf("here we merge 1\n");
        merge(arr,2*len_per_thread,3*len_per_thread,len,me()*len_per_thread);
        barrier_wait(&merge_barrier);
    }
    
}