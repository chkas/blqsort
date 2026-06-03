// SPDX-License-Identifier: MIT
// (c) christof.kaser@gmail.com

#ifndef BLQSORT_THR_H
#define BLQSORT_THR_H

#ifndef BLQS_PREFIX
#define BLQS_PREFIX bl
#endif

#ifndef BLQS_CMP
#define BLQS_CMP(a, b) ((a) < (b))
#endif

#define BLQS_CAT2(a,b) a##b
#define BLQS_CAT(a,b) BLQS_CAT2(a,b)
#define BLQS(x) BLQS_CAT(BLQS_PREFIX, x)

#include <stddef.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>
#include <stdlib.h>

static BLQS_TYPE* BLQS(partition)(BLQS_TYPE* left, BLQS_TYPE* right);
static void BLQS(sorting_network)(BLQS_TYPE* left, int size);
static void BLQS(heap_sort)(BLQS_TYPE* left, BLQS_TYPE* right);
static BLQS_TYPE* BLQS(partition_small)(BLQS_TYPE* left, BLQS_TYPE* right);

#define SMALLPART 1024

static int BLQS(max_threads);
static atomic_int BLQS(n_threads);

static pthread_mutex_t BLQS(mtx) = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t BLQS(cond) = PTHREAD_COND_INITIALIZER;

static void* BLQS(sort_thr)(void *arg);

// ------------------------------------------------------------

static void BLQS(sortr)(BLQS_TYPE* left, BLQS_TYPE* right) {

	while (1) {
		ptrdiff_t partsz = right - left;

		if (partsz <= 11) {
			BLQS(sorting_network)(left, partsz);
			return;
		}

		BLQS_TYPE* mid;

		if (partsz <= SMALLPART)
			mid = BLQS(partition_small)(left, right);
		else {
			mid = BLQS(partition)(left, right);

			if ((mid - left) * 16 < partsz) {
				BLQS(heap_sort)(left, right);
				return;
			}
		}

		if (mid - left > 1000000 && BLQS(n_threads) < BLQS(max_threads)) {

			BLQS_TYPE** thrdata = malloc(2 * sizeof(BLQS_TYPE*));
			if (thrdata) {

				thrdata[0] = left;
				thrdata[1] = mid - 1;

				pthread_t thread;
				BLQS(n_threads) += 1;

				if (pthread_create(&thread, NULL, BLQS(sort_thr), thrdata) == 0) {
					pthread_detach(thread);
					left = mid + 1;
					continue;
				}

				BLQS(n_threads) -= 1;
				free(thrdata);
			}
		}

		if (mid - left < right - mid) {
			BLQS(sortr)(left, mid - 1);
			left = mid + 1;
		}
		else {
			BLQS(sortr)(mid + 1, right);
			right = mid - 1;
		}
	}
}

// ------------------------------------------------------------

static void* BLQS(sort_thr)(void *arg) {

	BLQS_TYPE* left = ((BLQS_TYPE**)arg)[0];
	BLQS_TYPE* right = ((BLQS_TYPE**)arg)[1];

	free(arg);

	BLQS(sortr)(left, right);

	pthread_mutex_lock(&BLQS(mtx));

	BLQS(n_threads) -= 1;

	if (BLQS(n_threads) == 0)
		pthread_cond_signal(&BLQS(cond));

	pthread_mutex_unlock(&BLQS(mtx));

	return NULL;
}

// ------------------------------------------------------------

static void BLQS(heap_sort)(BLQS_TYPE* left, BLQS_TYPE* right) {

	long n = right - left + 1;
	if (n < 2) return;

	for (long i = n / 2, j;;) {
		BLQS_TYPE k;

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
			if (child + 1 < n && BLQS_CMP(left[child], left[child + 1])) child++;
			if (!BLQS_CMP(k, left[child])) break;
			left[j] = left[child];
			j = child;
		}

		left[j] = k;
	}
}

// ------------------------------------------------------------

#define sort2(a, b) do { \
	BLQS_TYPE x = a; \
	BLQS_TYPE y = b; \
	unsigned m = BLQS_CMP(x, y); \
	a = m ? x : y; \
	b = m ? y : x; \
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

#define sort8(a,b,c,d,e,f,g,h) do { \
	sort2(a,b); sort2(c,d); sort2(e,f); sort2(g,h); \
	sort2(a,c); sort2(b,d); sort2(e,g); sort2(f,h); \
	sort2(b,c); sort2(f,g); \
	sort2(a,e); sort2(b,f); sort2(c,g); sort2(d,h); \
	sort2(c,e); sort2(d,f); \
	sort2(b,c); sort2(d,e); sort2(f,g); \
} while (0)

#define sort9(a,b,c,d,e,f,g,h,i) do { \
	sort2(a,d); sort2(b,h); sort2(c,f); sort2(e,i); \
	sort2(a,h); sort2(c,e); sort2(d,i); sort2(f,g); \
	sort2(a,c); sort2(b,d); sort2(e,f); sort2(h,i); \
	sort2(b,e); sort2(d,g); sort2(f,h); \
	sort2(a,b); sort2(c,e); sort2(d,f); sort2(g,i); \
	sort2(c,d); sort2(e,f); sort2(g,h); \
	sort2(b,c); sort2(d,e); sort2(f,g); \
} while (0)

#define sort10(a,b,c,d,e,f,g,h,i,j) do { \
	sort2(a,i); sort2(b,j); sort2(c,h); sort2(d,f); sort2(e,g); \
	sort2(a,c); sort2(b,e); sort2(f,i); sort2(h,j); \
	sort2(a,d); sort2(c,e); sort2(f,h); sort2(g,j); \
	sort2(a,b); sort2(d,g); sort2(i,j); \
	sort2(b,f); sort2(c,d); sort2(e,i); sort2(g,h); \
	sort2(b,c); sort2(d,f); sort2(e,g); sort2(h,i); \
	sort2(c,d); sort2(e,f); sort2(g,h); \
	sort2(d,e); sort2(f,g); \
} while (0)

#define sort11(a,b,c,d,e,f,g,h,i,j,k) do { \
	sort2(a,j); sort2(b,g); sort2(c,e); sort2(d,h); sort2(f,i); \
	sort2(a,b); sort2(d,f); sort2(e,k); sort2(g,j); sort2(h,i); \
	sort2(b,d); sort2(c,f); sort2(e,h); sort2(i,k); \
	sort2(a,e); sort2(b,c); sort2(d,h); sort2(f,j); sort2(g,i); \
	sort2(a,b); sort2(c,g); sort2(e,f); sort2(h,i); sort2(j,k); \
	sort2(c,e); sort2(d,g); sort2(f,h); sort2(i,j); \
	sort2(b,c); sort2(d,e); sort2(f,g); sort2(h,i); \
	sort2(c,d); sort2(e,f); sort2(g,h); \
} while (0)

#define sort12(a,b,c,d,e,f,g,h,i,j,k,l) do { \
	sort2(a,i); sort2(b,h); sort2(c,g); sort2(d,l); sort2(e,k); sort2(f,j); \
	sort2(a,c); sort2(b,e); sort2(d,f); sort2(g,i); sort2(h,k); sort2(j,l); \
	sort2(a,b); sort2(c,j); sort2(e,h); sort2(f,g); sort2(k,l); \
	sort2(b,d); sort2(c,h); sort2(e,j); sort2(i,k); \
	sort2(a,b); sort2(c,d); sort2(e,f); sort2(g,h); sort2(i,j); sort2(k,l); \
	sort2(b,c); sort2(d,f); sort2(g,i); sort2(j,k); \
	sort2(c,e); sort2(d,g); sort2(f,i); sort2(h,j); \
	sort2(b,c); sort2(d,e); sort2(f,g); sort2(h,i); sort2(j,k); \
} while (0)

static void BLQS(sorting_network)(BLQS_TYPE* left, int n) {
	switch (n) {
	case 11: sort12(left[0],left[1],left[2],left[3],left[4],left[5],left[6],left[7],left[8],left[9],left[10],left[11]); break;
	case 10: sort11(left[0],left[1],left[2],left[3],left[4],left[5],left[6],left[7],left[8],left[9],left[10]); break;
	case 9: sort10(left[0],left[1],left[2],left[3],left[4],left[5],left[6],left[7],left[8],left[9]); break;
	case 8: sort9(left[0],left[1],left[2],left[3],left[4],left[5],left[6],left[7],left[8]); break;
	case 7: sort8(left[0],left[1],left[2],left[3],left[4],left[5],left[6],left[7]); break;
	case 6: sort7(left[0],left[1],left[2],left[3],left[4],left[5],left[6]); break;
	case 5: sort6(left[0],left[1],left[2],left[3],left[4],left[5]); break;
	case 4: sort5(left[0],left[1],left[2],left[3],left[4]); break;
	case 3: sort4(left[0],left[1],left[2],left[3]); break;
	case 2: sort3(left[0],left[1],left[2]); break;
	case 1: sort2(left[0],left[1]); break;
	default: break;
	}
}

// ------------------------------------------------------------

#define med5(a,b,c,d,e) do { \
	sort2(a,b); sort2(c,d); sort2(a,c); \
	sort2(b,d); sort2(b,c); sort2(c,e); \
	sort2(b,c); \
} while(0)

static BLQS_TYPE* BLQS(partition_small)(BLQS_TYPE* left, BLQS_TYPE* right) {
	BLQS_TYPE* outerleft = left;
	BLQS_TYPE* pivp = left + (right - left) / 2;

	BLQS_TYPE l1 = left[1], l2 = left[2];
	BLQS_TYPE piv = *pivp;
	BLQS_TYPE r1 = right[-1], r0 = *right;
	med5(l1, l2, piv, r1, r0);
	left[1] = l1; left[2] = l2;
	right[-1] = r1; *right = r0;

	left += 3;
	right -= 2;

	*pivp = *outerleft;

	BLQS_TYPE swbuf[SMALLPART];
	BLQS_TYPE* sw = swbuf;
	BLQS_TYPE* lwr = left;

	while (left <= right) {
		unsigned h = BLQS_CMP(*left, piv);
		*lwr = *sw = *left++;
		lwr += h;
		sw += !h;
	}

	memcpy(lwr, swbuf, (sw - swbuf) * sizeof(BLQS_TYPE));

	lwr -= 1;
	*outerleft = *lwr;
	*lwr = piv;

	return lwr;
}

#define SWSZ 1024
#define UNROLL 16

static BLQS_TYPE* BLQS(partition)(BLQS_TYPE* left, BLQS_TYPE* right) {

	BLQS_TYPE* outerleft = left;
	BLQS_TYPE* pivp = left + (right - left) / 2;

	BLQS_TYPE piv = *pivp;

	med5(left[3], left[4], left[1], left[5], left[6]);
	med5(left[11], left[12], left[2], left[13], left[14]);
	med5(pivp[-20], pivp[-10], piv, pivp[10], pivp[20]);
	med5(right[-6], right[-7], right[-1], right[-8], right[-9]);
	med5(right[-15], right[-14], right[0], right[-13], right[-12]);
	med5(left[1], left[2], piv, right[-1], right[0]);

	left += 3;
	right -= 2;

	*pivp = *outerleft;

	BLQS_TYPE swbuf[SWSZ];

	BLQS_TYPE* lwr = left;
	BLQS_TYPE* rwr = right;
	BLQS_TYPE* sw = swbuf;

	while (sw < swbuf + SWSZ - UNROLL && left <= right - UNROLL) {
		for (int i = UNROLL; i--;) {
			unsigned h = BLQS_CMP(*right, piv);
			*rwr = *sw = *right--;
			rwr -= !h;
			sw += h;
		}
	}

	while (sw < swbuf + SWSZ - UNROLL && left <= right) {
		unsigned h = BLQS_CMP(*right, piv);
		*rwr = *sw = *right--;
		rwr -= !h;
		sw += h;
	}

	while (left <= right - UNROLL) {
		while (rwr > right + UNROLL && left <= right - UNROLL) {
			for (int i = UNROLL; i--;) {
				unsigned h = BLQS_CMP(*left, piv);
				*lwr = *rwr = *left++;
				lwr += h;
				rwr -= !h;
			}
		}

		while (lwr < left - UNROLL && left <= right - UNROLL) {
			for (int i = UNROLL; i--;) {
				unsigned h = BLQS_CMP(*right, piv);
				*rwr = *lwr = *right--;
				rwr -= !h;
				lwr += h;
			}
		}
	}

	while (rwr > right && left <= right) {
		unsigned h = BLQS_CMP(*left, piv);
		*lwr = *rwr = *left++;
		lwr += h;
		rwr -= !h;
	}

	while (left <= right) {
		unsigned h = BLQS_CMP(*right, piv);
		*rwr = *lwr = *right--;
		rwr -= !h;
		lwr += h;
	}

	memcpy(lwr, swbuf, (sw - swbuf) * sizeof(BLQS_TYPE));

	*outerleft = *rwr;
	*rwr = piv;
	return rwr;
}

#undef SWSZ
#undef UNROLL

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

// ------------------------  public API  -----------------------------

static void BLQS(qsort)(BLQS_TYPE* data, int len) {

	int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);

	if (n_cpus > 0) BLQS(max_threads) = n_cpus * 2;
	else BLQS(max_threads) = 8;

	pthread_t thread;

	BLQS_TYPE** thrdata = malloc(2 * sizeof(BLQS_TYPE*));
	if (!thrdata) goto heap_sort_label;

	thrdata[0] = data;
	thrdata[1] = data + len - 1;

	BLQS(n_threads) = 1;

	if (pthread_create(&thread, NULL, BLQS(sort_thr), thrdata) != 0)
		goto heap_sort_label;

	pthread_mutex_lock(&BLQS(mtx));

	while (BLQS(n_threads) != 0)
		pthread_cond_wait(&BLQS(cond), &BLQS(mtx));

	pthread_mutex_unlock(&BLQS(mtx));

	return;

heap_sort_label:
	BLQS(heap_sort)(data, data + len - 1);
}

#endif
