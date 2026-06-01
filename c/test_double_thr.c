// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BLQS_CMP(a, b) ((a) < (b))
#define BLQS_TYPE double
#include "blqsort_thr.h"

double ts(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}
#define SIZE (50 * 1000000)
double data[SIZE];

int main(void) {

	srand(time(NULL));
	for (int i = 0; i < SIZE; i++) data[i] = rand();
	printf("Sorting %d million numbers  with threaded blqs ...\n", SIZE / 1000000);
	double a =  ts();
	blqsort(data, SIZE);
	printf("%.2fs\n", ts() - a);
	return 0;
}
