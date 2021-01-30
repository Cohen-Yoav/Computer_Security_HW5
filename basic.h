#include "util.h"

#ifndef BASIC_H_
#define BASIC_H_

CYCLES main_memory_access_latency();
CYCLES cache_access_latency();

void bubbleSort(CYCLES arr[], int length);
void swap(CYCLES *xp, CYCLES *yp);

#endif
