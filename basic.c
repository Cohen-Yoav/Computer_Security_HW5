#include "basic.h"
#include <stdio.h>

#define MAX_ITER 1000 
#define MIN_ITER 100

static CYCLES internal_memory_access_latency_check(bool is_flush)
{
	int cur_med = 0, prev_med = 0, k = 10, i;
	ADDR_PTR check_addr = (ADDR_PTR) &k;
	CYCLES access_latency[MAX_ITER];

	for (i = 0; i < MAX_ITER; i++) {
		prev_med = cur_med;
		if (is_flush){
			clflush((ADDR_PTR)check_addr); 
		}
		access_latency[i] = measure_access_time_to_addr((ADDR_PTR)check_addr);

		bubbleSort(access_latency, i + 1); 
		cur_med = access_latency[(i / 2)]; 

		if (cur_med == prev_med && i > MIN_ITER) {
			break;
		}
	}

	printf("total number of iterations: %d\n",i + 1);
	printf("median memory access time recoreded: %d\n",cur_med);
	printf("minimum memory access time recoreded: %d\n",access_latency[0]);
	printf("maximum memory access time recoreded: %d\n\n",access_latency[i]);
	
	return cur_med;
}

CYCLES main_memory_access_latency()
{
	printf("main_memory_access_latency results:\n");
	return internal_memory_access_latency_check(true);
}

CYCLES cache_access_latency()
{
	printf("cache_access_latency results:\n");
	return internal_memory_access_latency_check(false);
}


int main()
{
	CYCLES mem_access_latency;
       	mem_access_latency = main_memory_access_latency();
	CYCLES cache_access_latency_check;
       	cache_access_latency_check = cache_access_latency();


	printf("median main memory access latency: %u\n", mem_access_latency);
	printf("median cache access latency: %u\n\n", cache_access_latency_check);
}


void bubbleSort(CYCLES arr[], int length)
{
	int i,j;
	bool is_swap;
	for (i = 0; i < length - 1; i++) {
		is_swap = false;
		for (j = 0; j < length - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				swap(&arr[j], &arr[j + 1]);
				is_swap = true;
			}
		}
		// the array is sorted
		if (!is_swap) {
			break;
		}
	}
}

// This function swaps values pointed by xp and yp 
void swap(CYCLES *xp, CYCLES *yp) 
{ 
    CYCLES temp = *xp; 
    *xp = *yp; 
    *yp = temp; 
} 