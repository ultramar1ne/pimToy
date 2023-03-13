#include <dpu.h>
#include <dpu_log.h>

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "mram-management.h"
#include "./support/common.h"
#include "./support/matrix.h"
#include "./support/params.h"
#include "./support/timer.h"
#include "./support/utils.h"

#ifndef DPU_BINARY
#define DPU_BINARY "./dpu_sort"
#endif

#define BufferSize 240

int comp (const void * elem1, const void * elem2) 
{
    int f = *((int*)elem1);
    int s = *((int*)elem2);
    if (f > s) return  1;
    if (f < s) return -1;
    return 0;
}



int main(void) {
  Timer timer;
  float cpuTime = 0.0f, dpuTime = 0.0f, retrieveTime = 0.0f;
  struct dpu_set_t set, dpu;
  int a[BufferSize];   int a_dpu[BufferSize];  int result[BufferSize];

  for(int i = 0; i<BufferSize; i++){
    a[i] = rand();
    a_dpu[i] = a[i];
  }
  startTimer(&timer);
  qsort (a, sizeof(a)/sizeof(*a), sizeof(*a), comp);
  stopTimer(&timer);
  cpuTime += getElapsedTime(timer);
  PRINT_INFO(1==1,"cpu qsort time %f",cpuTime);

  struct mram_heap_allocator_t allocator;
  init_allocator(&allocator);
  DPU_ASSERT(dpu_alloc(1, NULL, &set));
  uint32_t dpuInVector_m = mram_heap_alloc(&allocator, BufferSize*sizeof(int));
  PRINT_INFO(1==1,"now cpu side finished");
  DPU_FOREACH(set, dpu) {
    DPU_ASSERT(dpu_load(dpu, DPU_BINARY, NULL));
    DPU_ASSERT(dpu_copy_to(dpu, "arr", 0, &a_dpu, sizeof(a_dpu)));
    startTimer(&timer);
    DPU_ASSERT(dpu_launch(dpu, DPU_SYNCHRONOUS));
    stopTimer(&timer);
    dpuTime += getElapsedTime(timer);
  }
  DPU_ASSERT(dpu_copy_from(dpu, "arr", 0, &result[0],sizeof(a)));
  PRINT_INFO(1,"dpu Run Time: %f",dpuTime);
 
  for(int i = 0; i<BufferSize; i++){if(0 || (result[i]!=a[i])){PRINT_INFO(1,"%dWRONG VALUE %d %d",i,result[i],a[i]);}}

  DPU_FOREACH(set, dpu) {
    DPU_ASSERT(dpu_log_read(dpu, stdout));
  }
  dpu_free(set);
  

  return 0;
}