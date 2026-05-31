// SPDX-License-Identifier: MIT
// (c) christof.kaser@gmail.com

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#define BLQS_CMP(a, b) ((a) < (b))
#define BLQS_TYPE double

#ifdef WITH_THREADS

#define NAME "Threaded Branchless Quicksort"
#include "blqsort_thr.h"

#else

#define NAME "Branchless Quicksort"
#include "blqsort.h"

#endif

unsigned chksum;
unsigned hash_el(void *p, int sz) {
	unsigned char* c = (unsigned char*)p;
	unsigned h = 0;
	for (int i = 0; i < sz; i++) h = (h * 131) + c[i];
	return h;
}
void init(BLQS_TYPE* data, int len) {
	chksum = 0;
	for (int i = 0; i < len; i++) {
		data[i] = rand();
		chksum += hash_el(&data[i], sizeof(BLQS_TYPE));
	}
}
void test(BLQS_TYPE* data, int len) {
	unsigned chks = hash_el(&data[0], sizeof(BLQS_TYPE));;
	for (int i = 1; i < len; i++) {
		if (data[i] < data[i - 1]) {
			printf("ERROR ORDER\n");
			break;
		}
		chks += hash_el(&data[i], sizeof(BLQS_TYPE));
	}
	if (chks != chksum) printf("ERROR CHKS\n");
}
double ts(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}
#define SIZE (50 * 1000000)
BLQS_TYPE data[SIZE];

int main(void) {

	srand(time(NULL));
	init(data, SIZE);
	printf("Sorting %d million numbers " NAME " ...\n", SIZE / 1000000);
	double a =  ts();
	blqsort(data, SIZE);
	printf("%.2fs\n", ts() - a);
	test(data, SIZE);

	return 0;
}
