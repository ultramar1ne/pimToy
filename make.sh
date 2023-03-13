gcc --std=c99 host_sort.c -o sort_host `dpu-pkg-config --cflags --libs dpu`;
dpu-upmem-dpurte-clang -DNR_TASKLETS=4 -O2 dpu_sort.c  -o dpu_sort;