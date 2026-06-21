// SPDX-License-Identifier: MIT
// (c) christof.kaser@gmail.com

#ifndef BLQSORT_THR_H
#define BLQSORT_THR_H

#if defined(__arm64__)
	#define PREFER_IF 1
#else
	#define PREFER_IF 0
#endif

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
#define min(a, b) ((a) < (b)) ? a : b

#define SMALLPART 1024
#define SWSZ 512
#define UNROLL 16

static void BLQS(heap_sort)(BLQS_TYPE* left, BLQS_TYPE* right) {

	ptrdiff_t n = right - left + 1;
	if (n < 2) return;
	for (ptrdiff_t i = n / 2, j;;) {
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
			ptrdiff_t child = j * 2 + 1;
			if (child + 1 < n && BLQS_CMP(left[child],left[child + 1])) child++;
			if (!BLQS_CMP(k,left[child])) break;
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

#define sort13(a,b,c,d,e,f,g,h,i,j,k,l,m) do { \
	sort2(a,m);sort2(b,k);sort2(c,j);sort2(d,h);sort2(f,l);sort2(g,i);\
	sort2(b,g);sort2(c,d);sort2(e,l);sort2(h,j);sort2(i,k);\
	sort2(a,e);sort2(b,c);sort2(d,g);sort2(h,i);sort2(j,k);sort2(l,m);\
	sort2(e,g);sort2(f,j);sort2(i,l);sort2(k,m);\
	sort2(a,f);sort2(d,i);sort2(e,h);sort2(g,l);sort2(j,k);\
	sort2(a,b);sort2(c,f);sort2(g,j);sort2(h,i);sort2(k,l);\
	sort2(b,d);sort2(c,e);sort2(f,g);sort2(j,k);\
	sort2(b,c);sort2(d,e);sort2(f,h);sort2(g,i);\
	sort2(c,d);sort2(e,f);sort2(g,h);sort2(i,j);\
	sort2(d,e);sort2(f,g);\
} while (0)

#define sort14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) do { \
	sort2(a,b);sort2(c,d);sort2(e,f);sort2(g,h);sort2(i,j);sort2(k,l);sort2(m,n);\
	sort2(a,c);sort2(b,d);sort2(e,i);sort2(f,j);sort2(k,m);sort2(l,n);\
	sort2(a,e);sort2(b,c);sort2(d,h);sort2(f,i);sort2(g,k);sort2(j,n);sort2(l,m);\
	sort2(a,g);sort2(b,f);sort2(d,j);sort2(e,k);sort2(h,n);sort2(i,m);\
	sort2(c,k);sort2(d,l);sort2(e,g);sort2(h,j);\
	sort2(b,d);sort2(c,i);sort2(f,l);sort2(g,h);sort2(k,m);\
	sort2(b,e);sort2(c,g);sort2(d,f);sort2(h,l);sort2(i,k);sort2(j,m);\
	sort2(c,e);sort2(d,g);sort2(f,i);sort2(h,k);sort2(j,l);\
	sort2(d,e);sort2(f,g);sort2(h,i);sort2(j,k);\
	sort2(g,h);\
} while(0)

#define sort15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) do { \
	sort2(b,c);sort2(d,k);sort2(e,o);sort2(f,i);sort2(g,n);sort2(h,m);sort2(j,l);\
	sort2(a,o);sort2(b,f);sort2(c,i);sort2(d,h);sort2(g,j);sort2(k,m);sort2(l,n);\
	sort2(a,h);sort2(b,g);sort2(c,j);sort2(e,k);sort2(f,l);sort2(i,n);sort2(m,o);\
	sort2(a,g);sort2(c,e);sort2(d,f);sort2(h,l);sort2(i,k);sort2(j,m);sort2(n,o);\
	sort2(a,d);sort2(b,c);sort2(e,h);sort2(f,j);sort2(g,i);sort2(k,l);sort2(m,n);\
	sort2(a,b);sort2(c,d);sort2(e,g);sort2(h,j);sort2(k,m);sort2(l,n);\
	sort2(b,c);sort2(d,f);sort2(i,k);sort2(l,m);\
	sort2(d,e);sort2(f,g);sort2(h,i);sort2(j,k);\
	sort2(c,d);sort2(e,f);sort2(g,h);sort2(i,j);sort2(k,l);\
	sort2(f,g);sort2(h,i);\
} while(0)

#define sort16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) do { \
	sort2(a,n);sort2(b,m);sort2(c,p);sort2(d,o);sort2(e,i);\
	sort2(f,g);sort2(h,l);sort2(j,k);\
	sort2(a,f);sort2(b,h);sort2(c,j);sort2(d,e);sort2(g,n);\
	sort2(i,o);sort2(k,p);sort2(l,m);\
	sort2(a,b);sort2(c,d);sort2(e,f);sort2(g,i);sort2(h,j);\
	sort2(k,l);sort2(m,n);sort2(o,p);\
	sort2(a,c);sort2(b,d);sort2(e,k);sort2(f,l);sort2(g,h);\
	sort2(i,j);sort2(m,o);sort2(n,p);\
	sort2(b,c);sort2(d,m);sort2(e,g);sort2(f,h);sort2(i,k);\
	sort2(j,l);sort2(n,o);\
	sort2(b,e);sort2(c,g);sort2(f,i);sort2(h,k);sort2(j,n);\
	sort2(l,o);\
	sort2(c,e);sort2(d,g);sort2(j,m);sort2(l,n);\
	sort2(d,f);sort2(g,i);sort2(h,j);sort2(k,m);\
	sort2(d,e);sort2(f,g);sort2(h,i);sort2(j,k);sort2(l,m);\
	sort2(g,h);sort2(i,j);\
} while (0)

#define sort17(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q) do { \
	sort2(a,l);sort2(b,p);sort2(c,k);sort2(d,f);sort2(e,g);\
	sort2(i,m);sort2(j,q);sort2(n,o);\
	sort2(a,g);sort2(b,n);sort2(c,i);sort2(e,o);sort2(f,p);\
	sort2(h,l);\
	sort2(a,i);sort2(d,h);sort2(e,j);sort2(g,q);sort2(k,l);\
	sort2(m,o);\
	sort2(a,c);sort2(b,e);sort2(f,g);sort2(h,n);sort2(i,j);\
	sort2(k,m);sort2(l,o);sort2(p,q);\
	sort2(a,d);sort2(c,f);sort2(g,l);sort2(h,k);sort2(j,n);\
	sort2(m,p);sort2(o,q);\
	sort2(a,b);sort2(d,e);sort2(f,k);sort2(g,j);sort2(h,i);\
	sort2(l,p);sort2(n,o);\
	sort2(b,c);sort2(d,h);sort2(e,i);sort2(g,m);sort2(l,n);\
	sort2(o,p);\
	sort2(b,d);sort2(c,h);sort2(e,f);sort2(j,l);sort2(k,m);\
	sort2(n,o);\
	sort2(c,d);sort2(e,g);sort2(f,h);sort2(i,k);\
	sort2(d,e);sort2(g,i);sort2(h,j);sort2(k,m);\
	sort2(f,g);sort2(h,i);sort2(j,k);sort2(l,m);\
	sort2(e,f);sort2(g,h);sort2(i,j);sort2(k,l);sort2(m,n);\
} while(0)

#define sort18(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r) do { \
	sort2(a,b);sort2(c,d);sort2(e,f);sort2(g,h);sort2(i,j);\
	sort2(k,l);sort2(m,n);sort2(o,p);sort2(q,r);\
	sort2(a,c);sort2(b,d);sort2(e,m);sort2(f,n);sort2(g,i);\
	sort2(j,l);sort2(o,q);sort2(p,r);\
	sort2(a,o);sort2(b,q);sort2(c,p);sort2(d,r);\
	sort2(a,g);sort2(b,k);sort2(c,j);sort2(h,q);sort2(i,p);\
	sort2(l,r);\
	sort2(b,e);sort2(d,j);sort2(f,h);sort2(i,o);sort2(k,m);\
	sort2(n,q);\
	sort2(a,b);sort2(c,f);sort2(d,n);sort2(e,o);sort2(h,j);\
	sort2(i,k);sort2(m,p);sort2(q,r);\
	sort2(b,c);sort2(d,f);sort2(e,g);sort2(l,n);sort2(m,o);\
	sort2(p,q);\
	sort2(e,i);sort2(f,m);sort2(g,k);sort2(h,l);sort2(j,n);\
	sort2(b,e);sort2(c,i);sort2(d,g);sort2(f,h);sort2(j,p);\
	sort2(k,m);sort2(l,o);sort2(n,q);\
	sort2(c,e);sort2(f,i);sort2(g,k);sort2(h,l);sort2(j,m);\
	sort2(n,p);\
	sort2(d,f);sort2(g,i);sort2(h,k);sort2(j,l);sort2(m,o);\
	sort2(d,e);sort2(f,g);sort2(h,i);sort2(j,k);sort2(l,m);\
	sort2(n,o);\
} while(0)

static void BLQS(sorting_network)(BLQS_TYPE* l, int partszm1_min_1) {
	switch (partszm1_min_1) {
	case 0: break;
	case 1: sort2(l[0],l[1]); break;
	case 2: sort3(l[0],l[1],l[2]); break;
	case 3: sort4(l[0],l[1],l[2],l[3]); break;
	case 4: sort5(l[0],l[1],l[2],l[3],l[4]); break;
	case 5: sort6(l[0],l[1],l[2],l[3],l[4],l[5]); break;
	case 6: sort7(l[0],l[1],l[2],l[3],l[4],l[5],l[6]); break;
	case 7: sort8(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7]); break;
	case 8: sort9(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8]); break;
	case 9: sort10(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9]); break;
	case 10: sort11(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10]); break;
	case 11: sort12(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],
		l[11]); break;
	case 12: sort13(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],
		l[12]); break;
	case 13: sort14(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],
		l[12],l[13]); break;
	case 14: sort15(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],
		l[12],l[13],l[14]); break;
	case 15: sort16(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],
		l[12],l[13],l[14],l[15]); break;
	case 16: sort17(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],
		l[12],l[13],l[14],l[15],l[16]); break;
	case 17: sort18(l[0],l[1],l[2],l[3],l[4],l[5],l[6],l[7],l[8],l[9],l[10],l[11],
		l[12],l[13],l[14],l[15],l[16],l[17]); break;
	}
}
#define med5(a,b,c,d,e) do { \
	sort2(a,b); sort2(c,d); sort2(a,c); \
	sort2(b,d); sort2(b,c); sort2(c,e); \
	sort2(b,c); \
} while(0)

#define med7(a, b, c, d, e, f, g) do { \
	sort2(a, b); sort2(c, d); sort2(a, c); \
	sort2(b, d); sort2(b, c); sort2(e, f); \
	sort2(e, g); sort2(a, e); \
	sort2(b, f); sort2(c, g); sort2(b, e); \
	sort2(c, e); \
	sort2(d, f); sort2(d, e); \
} while(0)

static BLQS_TYPE* BLQS(partition_small)(BLQS_TYPE* left, BLQS_TYPE* right) {

	BLQS_TYPE* outerleft = left;
	BLQS_TYPE* pivp = left + 8;

	BLQS_TYPE piv = *pivp;

	BLQS_TYPE l1 = left[1],l2 = left[2],l3 = left[3];
	BLQS_TYPE r2 = right[-2], r1 = right[-1], r0 = *right;
	med7(l1,l2,l3, piv, r2, r1, r0);
	left[1] = l1; left[2] = l2; left[3] = l3;
	right[-2] = r2; right[-1] = r1; *right = r0;
	left += 4;
	right -= 3;

	*pivp = *outerleft;

	BLQS_TYPE swbuf[SMALLPART];
	BLQS_TYPE* sw = swbuf;
	BLQS_TYPE* lwr = left;

	while (right - left >= UNROLL) for (int i = UNROLL; i--;) {
#if PREFER_IF
		BLQS_TYPE x = *left++;
		if (BLQS_CMP(x, piv)) *lwr++ = x;
		else *sw++ = x;
#else
		int h = BLQS_CMP(*left, piv);
		*lwr = *sw = *left++; lwr += h; sw += !h;
#endif
	}
	while (left <= right) {
#if PREFER_IF
		BLQS_TYPE x = *left++;
		if (BLQS_CMP(x, piv)) *lwr++ = x;
		else *sw++ = x;
#else
		int h = BLQS_CMP(*left, piv);
		*lwr = *sw = *left++;
		lwr += h; sw += !h;
#endif
	}
	memcpy(lwr, swbuf, (sw - swbuf) * sizeof(BLQS_TYPE));
	lwr -= 1;
	*outerleft = *lwr;
	*lwr = piv;
	return lwr;
}

// *****************************************************************************

static BLQS_TYPE* BLQS(partition)(BLQS_TYPE* left, BLQS_TYPE* right) {
	BLQS_TYPE* outerleft = left;
	BLQS_TYPE* pivp = left + (right - left) / 2;

	BLQS_TYPE piv = *pivp;

	med5(left[1],left[2],left[3],left[4],left[5]);
	med5(left[11],left[12],left[13],left[14],left[15]);
	med5(left[21],left[22],left[23],left[24],left[25]);
	med5(pivp[-2], pivp[-1], piv, pivp[1], pivp[2]);
	med5(right[-24], right[-23], right[-22], right[-21], right[-20]);
	med5(right[-14], right[-13], right[-12], right[-11], right[-10]);
	med5(right[-4], right[-3], right[-2], right[-1], right[0]);
	med7(left[3],left[13],left[23], piv, right[-22], right[-12], right[-2]);

	left += 1;
	*pivp = *outerleft;

	BLQS_TYPE swbuf[SWSZ];
	BLQS_TYPE *rwr = right, *sw = swbuf;
	BLQS_TYPE *lwr = left;

	while (UNROLL < SWSZ - (sw - swbuf) && left < right - UNROLL) {
		ptrdiff_t avail = min(right - left, SWSZ - (sw - swbuf));
		BLQS_TYPE* endp = right - avail;
		while (right > endp + UNROLL) {
			for (int i = UNROLL; i--;) {
#if PREFER_IF
				BLQS_TYPE x = *right--;
				if (BLQS_CMP(x, piv)) *sw++ = x;
				else *rwr-- = x;
#else
				int h = BLQS_CMP(*right, piv);
				*rwr = *sw = *right--;
				rwr -= !h; sw += h;
#endif
			}
		}
	}

	while (right - left >= UNROLL &&
			(rwr - right > UNROLL || left - lwr > UNROLL)) {

		while (rwr - right > UNROLL && right - left >= UNROLL) {
			for (int i = UNROLL; i--;) {
#if PREFER_IF
				BLQS_TYPE x = *left++;
				if (BLQS_CMP(x, piv)) *lwr++ = x;
				else *rwr-- = x;
#else
				int h = BLQS_CMP(*left, piv);
				*lwr = *rwr = *left++;
				lwr += h; rwr -= !h;
#endif
			}
		}
		while (left - lwr > UNROLL && right - left >= UNROLL) {
			for (int i = UNROLL; i--;) {
#if PREFER_IF
				BLQS_TYPE x = *right--;
				if (BLQS_CMP(x, piv)) *lwr++ = x;
				else *rwr-- = x;
#else
				int h = BLQS_CMP(*right, piv);
				*rwr = *lwr = *right--;
				rwr -= !h; lwr += h;
#endif
			}
		}
	}
	while ((lwr < left||rwr > right) && left <= right) {
		while (rwr > right && left <= right) {
			BLQS_TYPE x = *left++;
			if (BLQS_CMP(x, piv)) *lwr++ = x;
			else *rwr-- = x;
		}
		while (lwr < left && left <= right) {
			BLQS_TYPE x = *right--;
			if (BLQS_CMP(x, piv)) *lwr++ = x;
			else *rwr-- = x;
		}
	}
	while (left <= right && !BLQS_CMP(*right, piv)) {
		right--;
		rwr--;
	}
	memcpy(lwr, swbuf, (sw - swbuf) * sizeof(BLQS_TYPE));
	*outerleft = *rwr;
	*rwr = piv;
	return rwr;
}

// *****************************************************************************

static void BLQS(smallsort)(BLQS_TYPE* left, BLQS_TYPE* right) {
	while (right - left > 17) {
		BLQS_TYPE* mid = BLQS(partition_small)(left, right);
		BLQS(smallsort)(left, mid - 1);
		left = mid + 1;
	}
	BLQS(sorting_network)(left, right - left);
}

#include <pthread.h>
#include <unistd.h>
#include <stdatomic.h>

static int BLQS(max_threads);
static atomic_int BLQS(n_threads);

static pthread_mutex_t BLQS(mtx) = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t BLQS(cond) = PTHREAD_COND_INITIALIZER;

static void* BLQS(sort_thr)(void *arg);

// ------------------------------------------------------------

static void BLQS(sortr)(BLQS_TYPE* left, BLQS_TYPE* right) {
	while (1) {
		ptrdiff_t partszm1 = right - left;
		if (partszm1 <= SMALLPART) break;
		BLQS_TYPE* mid = BLQS(partition)(left, right);

		if (mid - left < partszm1 / 16) {
			if (mid > left) BLQS(sortr)(left, mid - 1);
			BLQS_TYPE piv = *mid;
			mid += 1;
			// collect duplicates
			for (BLQS_TYPE* p = mid; p <= right; p++) {
				if (!BLQS_CMP(piv, *p)) {
					BLQS_TYPE h = *mid;
					*mid = *p;
					*p = h;
					mid++;
				}
			}
			left = mid;
			if (right - left < SMALLPART) break;

			// second chance before fallback to heapsort
			mid = BLQS(partition)(left, right);
			if (mid - left < (right - left) / 16) {
				if (mid > left) BLQS(heap_sort)(left, mid - 1);
				if (mid < right) BLQS(heap_sort)(mid + 1, right);
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
	BLQS(smallsort)(left, right);
}

#undef SWSZ
#undef UNROLL
#undef SMALLPART

#undef med5
#undef med7

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
#undef sort13
#undef sort14
#undef sort15
#undef sort16
#undef sort17
#undef sort18

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

// ------------------------  public API  -----------------------------

static void BLQS(qsort)(BLQS_TYPE* data, int len) {

	if (len < 2) return;
	int n_cpus = sysconf(_SC_NPROCESSORS_ONLN);
	for (BLQS_TYPE* p = data + 1; p < data + len; p++) {
		if (BLQS_CMP(*p, *(p - 1))) goto not_sorted;
	}
	return;
not_sorted:
	if (n_cpus > 0) BLQS(max_threads) = n_cpus * 2;
	else BLQS(max_threads) = 8;

	BLQS_TYPE** thrdata = malloc(2 * sizeof(BLQS_TYPE*));
	if (!thrdata) goto no_thread;
	thrdata[0] = data;
	thrdata[1] = data + len - 1;

	BLQS(n_threads) = 1;
	pthread_t thread;
	if (pthread_create(&thread, NULL, BLQS(sort_thr), thrdata) != 0)
		goto no_thread;

	pthread_mutex_lock(&BLQS(mtx));

	while (BLQS(n_threads) != 0)
		pthread_cond_wait(&BLQS(cond), &BLQS(mtx));

	pthread_mutex_unlock(&BLQS(mtx));
	return;

no_thread:
	BLQS(max_threads) = 0;
	BLQS(sortr)(data, data + len - 1);
}

#endif
