// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

struct entry {
	int id;
	int value;
};
#define BLQS_CMP(a, b) (((a).id) < ((b).id))
#define BLQS_TYPE struct entry
#include "blqsort.h"

#define SIZE (50 * 1000000)
struct entry data[SIZE];

void init(void) {
	for (int i = 0; i < SIZE; i++) {
		data[i].id = rand();
		data[i].value = i;
	}
}
double ts(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int main(void) {

	srand(time(NULL));
	init();
	printf("Sorting %d million structs with blqs ...\n", SIZE / 1000000);
	double a =  ts();
	blqsort(data, SIZE);
	printf("%.2fs\n", ts() - a);
	return 0;
}
