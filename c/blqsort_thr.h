// SPDX-License-Identifier: MIT
// (c) christof.kaser@gmail.com

#ifndef BLQSORT_THR_H
#define BLQSORT_THR_H

#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

static TYPE* partition(TYPE* left, TYPE* right);
static void sorting_network(TYPE* left, int size);
static void heap_sort(TYPE* left, TYPE* right);
static TYPE* partition_small(TYPE* left, TYPE* right);

#define SMALLPART 1024

static int max_threads;
static atomic_int n_threads;

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static void* sort_thr(void *arg);

static void sortr(TYPE* left, TYPE* right) {

	while (1) {
		ptrdiff_t partsz = right - left;
		if (partsz <= 11) {
			sorting_network(left, partsz);
			return;
		}
		TYPE* mid;
		if (partsz <= SMALLPART) mid = partition_small(left, right);
		else {
			mid = partition(left, right);
			if ((mid - left) * 16 < partsz) {
				heap_sort(left, right);
				return;
			}
		}
		// soft limit: may exceed max_threads
		if (mid - left > 1000000 && n_threads < max_threads) {
			// start a new thread - max_threads is a soft limit
			TYPE** thrdata = malloc(2 * sizeof(TYPE*));
			if (thrdata) {

				thrdata[0] = left;
				thrdata[1] = mid - 1;

				pthread_t thread;
				n_threads += 1;
				if (pthread_create(&thread, NULL, sort_thr, thrdata) == 0) {
					pthread_detach(thread);
					left = mid + 1;
					continue;
				}
				n_threads -= 1;
				free(thrdata);
			}
		}
		if (mid - left < right - mid) {
			sortr(left, mid - 1);
			left = mid + 1;
		}
		else {
			sortr(mid + 1, right);
			right = mid - 1;
		}
	}
}

static void* sort_thr(void *arg) {

	TYPE* left = ((TYPE**)arg)[0];
	TYPE* right = ((TYPE**)arg)[1];
	free(arg);
	sortr(left, right);
	pthread_mutex_lock(&mtx);
	n_threads -= 1;
	if (n_threads == 0) pthread_cond_signal(&cond);
	pthread_mutex_unlock(&mtx);
	return NULL;
}

void blqsort(TYPE* data, int len) {

	int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);
	// printf("CPUs: %d\n", n_cpus);
	if (n_cpus > 0) max_threads = n_cpus * 2;
	else max_threads = 8;

	pthread_t thread;
	TYPE** thrdata = malloc(2 * sizeof(TYPE*));
	if (thrdata == NULL) goto heap_sort;
	thrdata[0] = data;
	thrdata[1] = data + len - 1;
	n_threads = 1;
	if (pthread_create(&thread, NULL, sort_thr, thrdata) != 0) goto heap_sort;

	pthread_mutex_lock(&mtx);
	while (n_threads != 0) pthread_cond_wait(&cond, &mtx);
	pthread_mutex_unlock(&mtx);
	return;
heap_sort:
	heap_sort(data, data + len - 1);
}

void heap_sort(TYPE* left, TYPE* right) {

	long n = right - left + 1;
	if (n < 2) return;
	for (long i = n / 2, j; ; ) {
		TYPE k;
		if (i > 0) {
			k = left[--i];
		}
		else {
			n -= 1;
			if (n == 0) return;
			k = left[n];
			left[n] = left[0];
		}
		j = i;
		while (j * 2 + 1 < n) {
			long child = j * 2 + 1;
			if (child + 1 < n && IS_LOWER(left[child], left[child + 1])) child++;
			if (!IS_LOWER(k, left[child])) break;
			left[j] = left[child];
			j = child;
		}
		left[j] = k;
	}
}

#define sort2(a, b) do {  \
	TYPE x = a; \
	TYPE y = b; \
	unsigned m = IS_LOWER(x, y); \
	a = m ? x : y;  \
	b = m ? y : x;  \
} while(0)

#define sort3(a, b, c) do { \
	sort2(a, b); sort2(b, c); sort2(a, b); \
} while(0)

#define sort4(a, b, c, d) do { \
	sort2(a, b); sort2(c, d); sort2(a, c); \
	sort2(b, d); sort2(b, c); \
} while(0)

#define sort5(a, b, c, d, e) do { \
	sort2(b, c); sort2(d, e); sort2(b, d); \
	sort2(a, c); sort2(a, d); sort2(c, e); \
	sort2(a, b); sort2(c, d); sort2(b, c); \
} while(0)

#define sort6(a, b, c, d, e, f) do { \
	sort2(a, b); sort2(c, d); sort2(e, f); \
	sort2(a, c); sort2(b, d); sort2(e, f); \
	sort2(a, e); sort2(b, f); sort2(c, e); \
	sort2(d, f); sort2(b, c); sort2(d, e); \
	sort2(c, d); \
} while(0)

#define sort7(a, b, c, d, e, f, g) do { \
	sort2(a, b); sort2(c, d); sort2(a, c); \
	sort2(b, d); sort2(b, c); sort2(e, f); \
	sort2(e, g); sort2(f, g); sort2(a, e); \
	sort2(b, f); sort2(c, g); sort2(b, e); \
	sort2(d, g); sort2(c, e); sort2(b, c); \
	sort2(d, f); sort2(d, e); \
} while(0)

#define sort8(a, b, c, d, e, f, g, h) do { \
	sort7(a, b, c, d, e, f, g); \
	sort2(g, h); sort2(f, g); sort2(e, f); \
	sort2(d, e); sort2(c, d); sort2(b, c); sort2(a, b); \
} while(0)

#define sort9(a, b, c, d, e, f, g, h, i) do { \
	sort8(a, b, c, d, e, f, g, h); \
	sort2(h, i); sort2(g, h); sort2(f, g); sort2(e, f); \
	sort2(d, e); sort2(c, d); sort2(b, c); sort2(a, b); \
} while(0)

#define sort10(a, b, c, d, e, f, g, h, i, j) do { \
	sort9(a, b, c, d, e, f, g, h, i); \
	sort2(i, j); sort2(h, i); sort2(g, h); sort2(f, g); sort2(e, f); \
	sort2(d, e); sort2(c, d); sort2(b, c); sort2(a, b); \
} while(0)

#define sort11(a,b,c,d,e,f,g,h,i,j,k) do { \
	sort10(a,b,c,d,e,f,g,h,i,j); \
	sort2(j,k); sort2(i,j); sort2(h,i); sort2(g,h); sort2(f,g); \
	sort2(e,f); sort2(d,e); sort2(c,d); sort2(b,c); sort2(a,b); \
} while(0)

#define sort12(a,b,c,d,e,f,g,h,i,j,k,l) do { \
	sort11(a,b,c,d,e,f,g,h,i,j,k); \
	sort2(k,l); sort2(j,k); sort2(i,j); sort2(h,i); sort2(g,h); \
	sort2(f,g); sort2(e,f); sort2(d,e); sort2(c,d); sort2(b,c); sort2(a,b); \
} while(0)

void sorting_network(TYPE* left, int partsz_min_1) {
	switch (partsz_min_1) {
	case 11:
		sort12(left[0], left[1], left[2], left[3], left[4], left[5], left[6], left[7],
			left[8], left[9], left[10], left[11]);
		break;
	case 10:
		sort11(left[0], left[1], left[2], left[3], left[4], left[5], left[6], left[7],
			left[8], left[9], left[10]);
		break;
	case 9:
		sort10(left[0], left[1], left[2], left[3], left[4], left[5], left[6], left[7],
			left[8], left[9]);
		break;
	case 8:
		sort9(left[0], left[1], left[2], left[3], left[4], left[5], left[6], left[7], left[8]);
		break;
	case 7:
		sort8(left[0], left[1], left[2], left[3], left[4], left[5], left[6], left[7]);
		break;
	case 6:
		sort7(left[0], left[1], left[2], left[3], left[4], left[5], left[6]);
		break;
	case 5:
		sort6(left[0], left[1], left[2], left[3], left[4], left[5]);
		break;
	case 4:
		sort5(left[0], left[1], left[2], left[3], left[4]);
		break;
	case 3:
		sort4(left[0], left[1], left[2], left[3]);
		break;
	case 2:
		sort3(left[0], left[1], left[2]);
		break;
	case 1:
		sort2(left[0], left[1]);
		break;
	case 0:
		break;
	}
}

#define med5(a, b, c, d, e) do { \
	sort2(a, b); sort2(c, d); sort2(a, c); \
	sort2(b, d); sort2(b, c); sort2(c, e); \
	sort2(b, c); \
} while(0)

TYPE* partition_small(TYPE* restrict  left, TYPE* restrict  right) {
	TYPE* outerleft = left;
	TYPE* pivp = left + (right - left) / 2;

	med5(left[1], left[2], *pivp, right[-1], *right);
	left += 3;
	right -= 2;

	TYPE piv = *pivp;
	*pivp = *outerleft;

	TYPE swbuf[SMALLPART];
	TYPE* sw = swbuf;
	TYPE* lwr = left;
	while (left <= right) {
		unsigned h = IS_LOWER(*left, piv); *lwr = *sw = *left++; lwr += h; sw += !h;
	}
	memcpy(lwr, swbuf, (sw - swbuf) * sizeof(TYPE));
	lwr -= 1;
	*outerleft = *lwr;
	*lwr = piv;
	return lwr;
}

#define SWSZ 1024
#define UNROLL 16

TYPE* partition(TYPE* restrict left, TYPE* restrict right) {

	TYPE* outerleft = left;
	TYPE* pivp = left + (right - left) / 2;

	med5(left[3], left[4], left[1], left[5], left[6]);
	med5(left[11], left[12], left[2], left[13], left[14]);
	med5(pivp[-20], pivp[-10], pivp[0], pivp[10], pivp[20]);
	med5(right[-6], right[-7], right[-1], right[-8], right[-9]);
	med5(right[-15], right[-14], right[0], right[-13], right[-12]);
	med5(left[1], left[2], pivp[0], right[-1], right[0]);
	left += 3;
	right -= 2;

	TYPE piv = *pivp;
	*pivp = *outerleft;

	TYPE swbuf[SWSZ];

	TYPE* lwr = left;
	TYPE* rwr = right;

	TYPE* sw = swbuf;
	while (sw < swbuf + SWSZ - UNROLL && left <= right - UNROLL) {
		for (int i = UNROLL; i--;) {
			unsigned h = IS_LOWER(*right, piv); *rwr = *sw = *right--; rwr -= !h; sw += h;
		}
	}
	while (sw < swbuf + SWSZ - UNROLL && left <= right) {
		unsigned h = IS_LOWER(*right, piv); *rwr = *sw = *right--; rwr -= !h; sw += h;
	}
	while (left <= right - UNROLL) {
		while (rwr > right + UNROLL && left <= right - UNROLL) {
			for (int i = UNROLL; i--;) {
				unsigned h = IS_LOWER(*left, piv); *lwr = *rwr = *left++; lwr += h; rwr -= !h;
			}
		}
		while (lwr < left - UNROLL && left <= right - UNROLL) {
			for (int i = UNROLL; i--;) {
				unsigned h = IS_LOWER(*right, piv); *rwr = *lwr = *right--; rwr -= !h; lwr += h;
			}
		}
	}
	while (rwr > right && left <= right) {
		unsigned h = IS_LOWER(*left, piv); *lwr = *rwr = *left++; lwr += h; rwr -= !h;
	}
	while (left <= right) {
		unsigned h = IS_LOWER(*right, piv); *rwr = *lwr = *right--; rwr -= !h; lwr += h;
	}
	memcpy(lwr, swbuf, (sw - swbuf) * sizeof(TYPE));

	*outerleft = *rwr;
	*rwr = piv;
	return rwr;
}

#undef med5
#undef sort2
#undef sort3
#undef sort4
#undef sort5
#undef sort6
#undef sort7
#undef sort8
#undef sort9
#undef sort10
#undef sort11
#undef sort12
#undef SMALLPART

#endif
