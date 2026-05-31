// SPDX-License-Identifier: MIT
// (c) christof.kaser@gmail.com

#ifndef BLQSORT_H
#define BLQSORT_H

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

static BLQS_TYPE* BLQS(partition)(BLQS_TYPE* left, BLQS_TYPE* right);
static void BLQS(sorting_network)(BLQS_TYPE* left, int size);
static void BLQS(heap_sort)(BLQS_TYPE* left, BLQS_TYPE* right);
static BLQS_TYPE* BLQS(partition_small)(BLQS_TYPE* left, BLQS_TYPE* right);

#define BLQS_SMALLPART 512

static void BLQS(sortr)(BLQS_TYPE* left, BLQS_TYPE* right) {
	while (1) {
		ptrdiff_t partsz = right - left;
		if (partsz <= 11) {
			BLQS(sorting_network)(left, partsz);
			return;
		}
		BLQS_TYPE* mid;
		if (partsz <= BLQS_SMALLPART) mid = BLQS(partition_small)(left, right);
		else {
			mid = BLQS(partition)(left, right);
			if ((mid - left) * 16 < partsz) {
				BLQS(heap_sort)(left, right);
				return;
			}
		}
		if (mid - left < right - mid) {
			BLQS(sortr)(left, mid - 1);
			left = mid + 1;
		} else {
			BLQS(sortr)(mid + 1, right);
			right = mid - 1;
		}
	}
}

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

#define sort2(a, b) do {  \
	BLQS_TYPE x = a; \
	BLQS_TYPE y = b; \
	unsigned m = BLQS_CMP(x, y); \
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

static void BLQS(sorting_network)(BLQS_TYPE* l, int partsz_min_1) {
	switch (partsz_min_1) {
	case 11: sort12(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11]); break;
	case 10: sort11(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10]); break;
	case 9: sort10(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9]); break;
	case 8: sort9(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8]); break;
	case 7: sort8(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7]); break;
	case 6: sort7(l[0],l[1],l[2],l[3],l[4],l[5],l[6]); break;
	case 5: sort6(l[0],l[1],l[2],l[3],l[4],l[5]); break;
	case 4: sort5(l[0],l[1],l[2],l[3],l[4]); break;
	case 3: sort4(l[0],l[1],l[2],l[3]); break;
	case 2: sort3(l[0],l[1],l[2]); break;
	case 1: sort2(l[0],l[1]); break;
	case 0: break;
	}
}

#define med5(a,b,c,d,e) do { \
	sort2(a,b); sort2(c,d); sort2(a,c); \
	sort2(b,d); sort2(b,c); sort2(c,e); \
	sort2(b,c); \
} while(0)

#define SWSZ 1024
#define UNROLL 16

static BLQS_TYPE* BLQS(partition_small)(BLQS_TYPE* restrict left, BLQS_TYPE* restrict right) {
	BLQS_TYPE* outerleft = left;
	BLQS_TYPE* pivp = left + (right - left) / 2;

	med5(left[1], left[2], *pivp, right[-1], *right);

	left += 3;
	right -= 2;

	BLQS_TYPE piv = *pivp;
	*pivp = *outerleft;

	BLQS_TYPE swbuf[BLQS_SMALLPART];
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

static BLQS_TYPE* BLQS(partition)(BLQS_TYPE* restrict left, BLQS_TYPE* restrict right) {

	BLQS_TYPE* outerleft = left;
	BLQS_TYPE* pivp = left + (right - left) / 2;

	med5(left[3], left[4], left[1], left[5], left[6]);
	med5(left[11], left[12], left[2], left[13], left[14]);
	med5(pivp[-20], pivp[-10], pivp[0], pivp[10], pivp[20]);
	med5(right[-6], right[-7], right[-1], right[-8], right[-9]);
	med5(right[-15], right[-14], right[0], right[-13], right[-12]);
	med5(left[1], left[2], pivp[0], right[-1], right[0]);

	left += 3;
	right -= 2;

	BLQS_TYPE piv = *pivp;
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
#undef BLQS_SMALLPART

// ------------------------  public API  -----------------------------

static void BLQS(qsort)(BLQS_TYPE* data, int len) {
	BLQS(sortr)(data, data + len - 1);
}

#endif
