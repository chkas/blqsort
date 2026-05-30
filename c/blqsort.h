// SPDX-License-Identifier: MIT
// (c) christof.kaser@gmail.com

#ifndef BLQSORT_H
#define BLQSORT_H

#include <stddef.h>
#include <string.h>

static TYPE* partition(TYPE* left, TYPE* right);
static void sorting_network(TYPE* left, int size);
static void heap_sort(TYPE* left, TYPE* right);
static TYPE* partition_small(TYPE* left, TYPE* right);

#define SMALLPART 512

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
		if (mid - left < right - mid) {
			sortr(left, mid - 1);
			left = mid + 1;
		} else {
			sortr(mid + 1, right);
			right = mid - 1;
		}
	}
}

void blqsort(TYPE* data, int len) {
	sortr(data, data + len - 1);
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

// https://bertdobbelaere.github.io/sorting_networks.html

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

void sorting_network(TYPE* l, int partsz_min_1) {
	switch (partsz_min_1) {
	case 11:
		sort12(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
			l[8], l[9], l[10], l[11]);
		break;
	case 10:
		sort11(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7], l[8], l[9], l[10]);
		break;
	case 9:
		sort10(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7], l[8], l[9]);
		break;
	case 8:
		sort9(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7], l[8]);
		break;
	case 7:
		sort8(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7]);
		break;
	case 6:
		sort7(l[0], l[1], l[2], l[3], l[4], l[5], l[6]);
		break;
	case 5:
		sort6(l[0], l[1], l[2], l[3], l[4], l[5]);
		break;
	case 4:
		sort5(l[0], l[1], l[2], l[3], l[4]);
		break;
	case 3:
		sort4(l[0], l[1], l[2], l[3]);
		break;
	case 2:
		sort3(l[0], l[1], l[2]);
		break;
	case 1:
		sort2(l[0], l[1]);
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
