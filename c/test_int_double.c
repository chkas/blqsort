// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BLQS_CMP(a, b) ((a) < (b))
#define BLQS_TYPE int
#include "blqsort_thr.h"

#undef BLQSORT_THR_H
#undef BLQS_PREFIX
#undef BLQS_TYPE

#define BLQS_PREFIX bldbl
#define BLQS_TYPE double
#include "blqsort_thr.h"

#define SIZE (50 * 1000000)

int data[SIZE];
double datadbl[SIZE];

double ts(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main(void) {
	double t;
	srand(time(NULL));

	for (int i = 0; i < SIZE; i++) data[i] = rand();
	printf("Sorting %d million int with threads ...\n", SIZE / 1000000);
	t =  ts();
	blqsort(data, SIZE);
	printf("%.2fs\n", ts() - t);

	for (int i = 0; i < SIZE; i++) datadbl[i] = rand();
	printf("Sorting %d million double with threads ...\n", SIZE / 1000000);
	t =  ts();
	bldblqsort(datadbl, SIZE);
	printf("%.2fs\n", ts() - t);

	return 0;
}
