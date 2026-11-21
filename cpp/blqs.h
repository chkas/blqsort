// SPDX-License-Identifier: MIT
// blqs.h - Branchless Quicksort
// (c) 2026 christof.kaser@gmail.com
//
#ifndef BLQS_H
#define BLQS_H

#if defined(__arm64__)
	#define PREFER_IF 1
#else
	#define PREFER_IF 0
#endif

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <type_traits>
#include <functional>
#include <utility>

namespace blqs {

constexpr int SMALLPART = 1024;
constexpr int SWSZ = 384;
constexpr int UNROLL = 16;

constexpr long BLSZ = 896;

template<typename T, typename Compare>
static inline void sort2(T& a, T& b, Compare comp) {
	T x = a; T y = b;
	bool m = comp(x, y);
	a = m ? x : y; b = m ? y : x;
}

template<typename T, typename Compare>
static inline void sort3(T& a, T& b, T& c, Compare comp) {
	sort2(a, b, comp); sort2(b, c, comp); sort2(a, b, comp);
}

template<typename T, typename Compare>
static inline void sort4(T& a, T& b, T& c, T& d, Compare comp) {
	sort2(a, b, comp); sort2(c, d, comp); sort2(a, c, comp);
	sort2(b, d, comp); sort2(b, c, comp);
}

template<typename T, typename Compare>
static inline void sort5(T& a, T& b, T& c, T& d, T& e, Compare comp) {
	sort2(b, c, comp); sort2(d, e, comp); sort2(b, d, comp);
	sort2(a, c, comp); sort2(a, d, comp); sort2(c, e, comp);
	sort2(a, b, comp); sort2(c, d, comp); sort2(b, c, comp);
}

template<typename T, typename Compare>
static inline void sort6(T& a, T& b, T& c, T& d, T& e, T& f, Compare comp) {
	sort2(a, b, comp); sort2(c, d, comp); sort2(e, f, comp);
	sort2(a, c, comp); sort2(b, d, comp); sort2(e, f, comp);
	sort2(a, e, comp); sort2(b, f, comp); sort2(c, e, comp);
	sort2(d, f, comp); sort2(b, c, comp); sort2(d, e, comp);
	sort2(c, d, comp);
}

template<typename T, typename Compare>
static inline void sort7(T& a, T& b, T& c, T& d, T& e, T& f, T& g, Compare comp) {
	sort2(a, g, comp); sort2(c, d, comp); sort2(e, f, comp);
	sort2(a, c, comp); sort2(b, e, comp); sort2(d, g, comp);
	sort2(a, b, comp); sort2(c, f, comp); sort2(d, e, comp);
	sort2(b, c, comp); sort2(e, g, comp);
	sort2(c, d, comp); sort2(e, f, comp);
	sort2(b, c, comp); sort2(d, e, comp); sort2(f, g, comp);
}

template<typename T, typename Compare>
static inline void sort8(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, Compare comp) {
	sort2(a,b,comp); sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp);
	sort2(a,c,comp); sort2(b,d,comp); sort2(e,g,comp); sort2(f,h,comp);
	sort2(b,c,comp); sort2(f,g,comp);
	sort2(a,e,comp); sort2(b,f,comp); sort2(c,g,comp); sort2(d,h,comp);
	sort2(c,e,comp); sort2(d,f,comp);
	sort2(b,c,comp); sort2(d,e,comp); sort2(f,g,comp);
}

template<typename T, typename Compare>
static inline void sort9(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, Compare comp) {
	sort2(a,d,comp); sort2(b,h,comp); sort2(c,f,comp); sort2(e,i,comp);
	sort2(a,h,comp); sort2(c,e,comp); sort2(d,i,comp); sort2(f,g,comp);
	sort2(a,c,comp); sort2(b,d,comp); sort2(e,f,comp); sort2(h,i,comp);
	sort2(b,e,comp); sort2(d,g,comp); sort2(f,h,comp);
	sort2(a,b,comp); sort2(c,e,comp); sort2(d,f,comp); sort2(g,i,comp);
	sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp);
	sort2(b,c,comp); sort2(d,e,comp); sort2(f,g,comp);
}

template<typename T, typename Compare>
static inline void sort10(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, Compare comp) {
	sort2(a,b,comp); sort2(c,f,comp); sort2(d,g,comp); sort2(e,h,comp); sort2(i,j,comp);
	sort2(a,g,comp); sort2(b,i,comp); sort2(c,e,comp); sort2(d,j,comp); sort2(f,h,comp);
	sort2(a,c,comp); sort2(b,d,comp); sort2(e,f,comp); sort2(g,i,comp); sort2(h,j,comp);
	sort2(a,b,comp); sort2(c,h,comp); sort2(d,f,comp); sort2(e,g,comp); sort2(i,j,comp);
	sort2(b,c,comp); sort2(d,e,comp); sort2(f,g,comp); sort2(h,i,comp);
	sort2(b,d,comp); sort2(c,e,comp); sort2(f,h,comp); sort2(g,i,comp);
	sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp);
}

template<typename T, typename Compare>
static inline void sort11(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k, Compare comp) {
	sort2(a,j,comp); sort2(b,g,comp); sort2(c,e,comp); sort2(d,h,comp); sort2(f,i,comp);
	sort2(a,b,comp); sort2(d,f,comp); sort2(e,k,comp); sort2(g,j,comp); sort2(h,i,comp);
	sort2(b,d,comp); sort2(c,f,comp); sort2(e,h,comp); sort2(i,k,comp);
	sort2(a,e,comp); sort2(b,c,comp); sort2(d,h,comp); sort2(f,j,comp); sort2(g,i,comp);
	sort2(a,b,comp); sort2(c,g,comp); sort2(e,f,comp); sort2(h,i,comp);
	sort2(j,k,comp);
	sort2(c,e,comp); sort2(d,g,comp); sort2(f,h,comp); sort2(i,j,comp);
	sort2(b,c,comp); sort2(d,e,comp); sort2(f,g,comp); sort2(h,i,comp);
	sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp);
}

template<typename T, typename Compare>
static inline void sort12(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k, T& l, Compare comp) {
	sort2(a,i,comp); sort2(b,h,comp); sort2(c,g,comp); sort2(d,l,comp); sort2(e,k,comp); sort2(f,j,comp);
	sort2(a,c,comp); sort2(b,e,comp); sort2(d,f,comp); sort2(g,i,comp); sort2(h,k,comp); sort2(j,l,comp);
	sort2(a,b,comp); sort2(c,j,comp); sort2(e,h,comp); sort2(f,g,comp); sort2(k,l,comp);
	sort2(b,d,comp); sort2(c,h,comp); sort2(e,j,comp); sort2(i,k,comp);
	sort2(a,b,comp); sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp); sort2(i,j,comp); sort2(k,l,comp);
	sort2(b,c,comp); sort2(d,f,comp); sort2(g,i,comp); sort2(j,k,comp);
	sort2(c,e,comp); sort2(d,g,comp); sort2(f,i,comp); sort2(h,j,comp);
	sort2(b,c,comp); sort2(d,e,comp); sort2(f,g,comp); sort2(h,i,comp); sort2(j,k,comp);
}
template<typename T, typename Compare>
static inline void sort13(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k, T& l, T& m,
						  Compare comp) {
	sort2(a,m,comp); sort2(b,k,comp); sort2(c,j,comp); sort2(d,h,comp); sort2(f,l,comp); sort2(g,i,comp);
	sort2(b,g,comp); sort2(c,d,comp); sort2(e,l,comp); sort2(h,j,comp); sort2(i,k,comp);
	sort2(a,e,comp); sort2(b,c,comp); sort2(d,g,comp); sort2(h,i,comp); sort2(j,k,comp); sort2(l,m,comp);
	sort2(e,g,comp); sort2(f,j,comp); sort2(i,l,comp); sort2(k,m,comp);
	sort2(a,f,comp); sort2(d,i,comp); sort2(e,h,comp); sort2(g,l,comp); sort2(j,k,comp);
	sort2(a,b,comp); sort2(c,f,comp); sort2(g,j,comp); sort2(h,i,comp); sort2(k,l,comp);
	sort2(b,d,comp); sort2(c,e,comp); sort2(f,g,comp); sort2(j,k,comp);
	sort2(b,c,comp); sort2(d,e,comp); sort2(f,h,comp); sort2(g,i,comp);
	sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp); sort2(i,j,comp);
	sort2(d,e,comp); sort2(f,g,comp);
}
template<typename T, typename Compare>
static inline void sort14(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k, T& l,
						T& m, T& n, Compare comp) {
	sort2(a,b,comp); sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp); sort2(i,j,comp); sort2(k,l,comp); sort2(m,n,comp);
	sort2(a,c,comp); sort2(b,d,comp); sort2(e,i,comp); sort2(f,j,comp); sort2(k,m,comp); sort2(l,n,comp);
	sort2(a,e,comp); sort2(b,c,comp); sort2(d,h,comp); sort2(f,i,comp); sort2(g,k,comp); sort2(j,n,comp); sort2(l,m,comp);
	sort2(a,g,comp); sort2(b,f,comp); sort2(d,j,comp); sort2(e,k,comp); sort2(h,n,comp); sort2(i,m,comp);
	sort2(c,k,comp); sort2(d,l,comp); sort2(e,g,comp); sort2(h,j,comp);
	sort2(b,d,comp); sort2(c,i,comp); sort2(f,l,comp); sort2(g,h,comp); sort2(k,m,comp);
	sort2(b,e,comp); sort2(c,g,comp); sort2(d,f,comp); sort2(h,l,comp); sort2(i,k,comp); sort2(j,m,comp);
	sort2(c,e,comp); sort2(d,g,comp); sort2(f,i,comp); sort2(h,k,comp); sort2(j,l,comp);
	sort2(d,e,comp); sort2(f,g,comp); sort2(h,i,comp); sort2(j,k,comp);
	sort2(g,h,comp);
}
template<typename T, typename Compare>
static inline void sort15(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k, T& l,
						T& m, T& n, T& o, Compare comp) {
	sort2(b,c,comp); sort2(d,k,comp); sort2(e,o,comp); sort2(f,i,comp); sort2(g,n,comp); sort2(h,m,comp); sort2(j,l,comp);
	sort2(a,o,comp); sort2(b,f,comp); sort2(c,i,comp); sort2(d,h,comp); sort2(g,j,comp); sort2(k,m,comp); sort2(l,n,comp);
	sort2(a,h,comp); sort2(b,g,comp); sort2(c,j,comp); sort2(e,k,comp); sort2(f,l,comp); sort2(i,n,comp); sort2(m,o,comp);
	sort2(a,g,comp); sort2(c,e,comp); sort2(d,f,comp); sort2(h,l,comp); sort2(i,k,comp); sort2(j,m,comp); sort2(n,o,comp);
	sort2(a,d,comp); sort2(b,c,comp); sort2(e,h,comp); sort2(f,j,comp); sort2(g,i,comp); sort2(k,l,comp); sort2(m,n,comp);
	sort2(a,b,comp); sort2(c,d,comp); sort2(e,g,comp); sort2(h,j,comp); sort2(k,m,comp); sort2(l,n,comp);
	sort2(b,c,comp); sort2(d,f,comp); sort2(i,k,comp); sort2(l,m,comp);
	sort2(d,e,comp); sort2(f,g,comp); sort2(h,i,comp); sort2(j,k,comp);
	sort2(c,d,comp); sort2(e,f,comp); sort2(g,h,comp); sort2(i,j,comp); sort2(k,l,comp);
	sort2(f,g,comp); sort2(h,i,comp);
}
template<typename T, typename Compare>
static inline void sort16(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k,
							T& l, T& m, T& n, T& o, T& p, Compare comp) {
	sort2(a,n,comp); sort2(b,m,comp); sort2(c,p,comp); sort2(d,o,comp); sort2(e,i,comp);
	sort2(f,g,comp); sort2(h,l,comp); sort2(j,k,comp);
	sort2(a,f,comp); sort2(b,h,comp); sort2(c,j,comp); sort2(d,e,comp); sort2(g,n,comp);
	sort2(i,o,comp); sort2(k,p,comp); sort2(l,m,comp);
	sort2(a,b,comp); sort2(c,d,comp); sort2(e,f,comp); sort2(g,i,comp); sort2(h,j,comp);
	sort2(k,l,comp); sort2(m,n,comp); sort2(o,p,comp);
	sort2(a,c,comp); sort2(b,d,comp); sort2(e,k,comp); sort2(f,l,comp); sort2(g,h,comp);
	sort2(i,j,comp); sort2(m,o,comp); sort2(n,p,comp);
	sort2(b,c,comp); sort2(d,m,comp); sort2(e,g,comp); sort2(f,h,comp); sort2(i,k,comp);
	sort2(j,l,comp); sort2(n,o,comp);
	sort2(b,e,comp); sort2(c,g,comp); sort2(f,i,comp); sort2(h,k,comp); sort2(j,n,comp);
	sort2(l,o,comp);
	sort2(c,e,comp); sort2(d,g,comp); sort2(j,m,comp); sort2(l,n,comp);
	sort2(d,f,comp); sort2(g,i,comp); sort2(h,j,comp); sort2(k,m,comp);
	sort2(d,e,comp); sort2(f,g,comp); sort2(h,i,comp); sort2(j,k,comp); sort2(l,m,comp);
	sort2(g,h,comp); sort2(i,j,comp);
}

template<typename T, typename Compare>
static inline void sorting_network(T* l, int partsz_min1, Compare comp) {
	switch (partsz_min1) {
	case 0: break;
	case 1:  sort2(l[0], l[1], comp); break;
	case 2:  sort3(l[0], l[1], l[2], comp); break;
	case 3:  sort4(l[0], l[1], l[2], l[3], comp); break;
	case 4:  sort5(l[0], l[1], l[2], l[3], l[4], comp); break;
	case 5:  sort6(l[0], l[1], l[2], l[3], l[4], l[5], comp); break;
	case 6:  sort7(l[0], l[1], l[2], l[3], l[4], l[5], l[6], comp); break;
	case 7:  sort8(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7], comp); break;
	case 8:  sort9(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		l[8], comp); break;
	case 9:  sort10(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		 l[8], l[9], comp); break;
	case 10: sort11(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		l[8], l[9], l[10], comp); break;
	case 11: sort12(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		l[8], l[9], l[10], l[11], comp); break;
	case 12: sort13(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		l[8], l[9], l[10], l[11], l[12], comp); break;
	case 13: sort14(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		l[8], l[9], l[10], l[11], l[12], l[13], comp); break;
	case 14: sort15(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		l[8], l[9], l[10], l[11], l[12], l[13], l[14], comp); break;
	case 15: sort16(l[0], l[1], l[2], l[3], l[4], l[5], l[6], l[7],
		l[8], l[9], l[10], l[11], l[12], l[13], l[14], l[15], comp); break;
	default: break;
	}
}

template<typename T, typename Compare>
inline void heap_sift_down(T* left, ptrdiff_t n, ptrdiff_t j, T& k, Compare& comp) {
	while (j * 2 + 1 < n) {
		ptrdiff_t child = j * 2 + 1;
		if (child + 1 < n && comp(left[child], left[child + 1])) child++;
		if (!comp(k, left[child])) break;
		left[j] = std::move(left[child]);
		j = child;
	}
	left[j] = std::move(k);
}

template<typename T, typename Compare>
static void heap_sort(T* left, T* right, Compare comp) {
	ptrdiff_t n = right - left + 1;
	if (n < 2) return;
	for (ptrdiff_t i = n / 2; ; ) {
		if (i > 0) {
			i--;
			T k = std::move(left[i]);
			heap_sift_down(left, n, i, k, comp);
		}
		else {
			n--;
			if (n == 0) return;
			T k = std::move(left[n]);
			left[n] = std::move(left[0]);
			heap_sift_down(left, n, 0, k, comp);
		}
	}
}
template<typename T, typename Compare>
static inline void med5(T& a, T& b, T& c, T& d, T& e, Compare comp) {
	sort2(a, b, comp); sort2(c, d, comp);
	sort2(a, c, comp); sort2(b, d, comp);
	sort2(b, c, comp); sort2(c, e, comp);
	sort2(b, c, comp);
}

template<typename T, typename Compare>
static inline void med7(T& a, T& b, T& c, T& d, T& e, T& f, T& g, Compare comp) {
	sort2(a, b, comp); sort2(c, d, comp); sort2(a, c, comp);
	sort2(b, d, comp); sort2(b, c, comp); sort2(e, f, comp);
	sort2(e, g, comp); sort2(a, e, comp);
	sort2(b, f, comp); sort2(c, g, comp); sort2(b, e, comp);
	sort2(c, e, comp);
	sort2(d, f, comp); sort2(d, e, comp);
}

template<typename T, typename Compare>
static inline T* partition_small(T* left, T* right, Compare comp) {
	static_assert(std::is_trivially_copyable_v<T>);
	T* pivp = left + (right - left) / 2;

	T piv = *pivp;
	T l1 = left[1], l2 = left[2], l3 = pivp[-1];
	T r2 = pivp[1], r1 = right[-1], r0 = *right;

	int cnt = !comp(l2, l1) + !comp(l3, l2) + !comp(piv, l3) + !comp(r2, piv);
	if (cnt == 4) {
		int nwrong = 0;
		for (T* pi = left + 1; pi <= right; pi++) {
			nwrong += comp(*pi, *(pi - 1));
			if (nwrong > 2) goto continue_partition;
		}
		if (nwrong == 0) return NULL;	// already sorted
		// insertion sort
		for (T* pi = left + 1; pi <= right; pi++) {
			T v = *pi;
			T* pj = pi;
			while (pj > left && comp(v, *(pj - 1))) {
				*pj = *(pj - 1);
				pj--;
			}
			*pj = v;
		}
		return NULL;
	}
continue_partition:
	med7(l1, l2, l3, piv, r2, r1, r0, comp);
	left[1] = l1; left[2] = l2; pivp[-1] = l3;
	pivp[1] = r2; right[-1] = r1; *right = r0;
	T* outerleft = left;
	left += 3;
	right -= 2;

	*pivp = *outerleft;
	T swbuf[SMALLPART];
	T *sw = swbuf, *lwr = left;

	while (right - left >= UNROLL) for (int i = UNROLL; i--;) {
#if PREFER_IF
		T x = *left++;
		if (comp(x, piv)) *lwr++ = x;
		else *sw++ = x;
#else
		int h = comp(*left, piv);
		*lwr = *sw = *left++;
		lwr += h; sw += !h;
#endif
	}
	while (left <= right) {
#if PREFER_IF
		T x = *left++;
		if (comp(x, piv)) *lwr++ = x;
		else *sw++ = x;
#else
		int h = comp(*left, piv);
		*lwr = *sw = *left++;
		lwr += h; sw += !h;
#endif
	}
	std::memcpy(lwr, swbuf, (sw - swbuf) * sizeof(T));
	lwr -= 1;
	*outerleft = *lwr;
	*lwr = piv;
	return lwr;
}

template<typename T, typename Compare>
static inline T* partition_large(T* left, T* right, Compare comp) {
	static_assert(std::is_trivially_copyable_v<T>);

	T* outerleft = left;
	T* pivp = left + (right - left) / 2;
	T piv = *pivp;

	med5(left[1], left[2], left[3], left[4], left[5], comp);
	med5(pivp[-10], pivp[-9], pivp[-8], pivp[-7], pivp[-6], comp);
	med5(pivp[-5], pivp[-3], piv, pivp[3], pivp[5], comp);
	med5(pivp[6], pivp[7], pivp[8], pivp[9], pivp[10], comp);
	med5(right[-4], right[-3], right[-2], right[-1], right[0], comp);
	med5(left[3], pivp[-8],  piv, pivp[8], right[-2] , comp);

	left += 1;
	*pivp = *outerleft;

	T swbuf_dummy[SWSZ + 1]; // to avoid a pointer-before-array UB
	T *swbuf = swbuf_dummy + 1;
	T *lwr = left, *rwr = right;
	T *right0 = right;
	T *sw = swbuf + SWSZ - 1;

	while (UNROLL < sw - swbuf + 1  && left < right - UNROLL) {
		ptrdiff_t avail = std::min(right - left, sw - swbuf + 1);
		T* endp = right - avail;
		while (right > endp + UNROLL) {
			for (int i = UNROLL; i--;) {
#if PREFER_IF
				T x = *right--;
				if (comp(x, piv)) *sw-- = x;
				else *rwr-- = x;
#else
				int h = comp(*right, piv);
				*rwr = *sw = *right--;
				rwr -= !h; sw -= h;
#endif
			}
		}
	}
	int swsz = swbuf + SWSZ - 1 - sw;

	if (swsz < SWSZ / 8 || right0 - right < SWSZ + SWSZ / 8) {
		// the data isn't that random, so the branch predictor works (lomuto)
		// includes the case of empty swap space, where this is necessary
		std::memcpy(right + 1, sw + 1, swsz * sizeof(T));
		right += swsz;
		while (left <= right) {
			if (comp(*left, piv)) left++;
			else {
				std::swap(*left, *right);
				right--;
			}
		}
		*outerleft = *right;
		*right = piv;
		return right;
	}

	while (right - left >= UNROLL &&
			(rwr - right > UNROLL || left - lwr > UNROLL)) {

		while (rwr - right > UNROLL && right - left >= UNROLL) {
			for (int i = UNROLL; i--;) {
#if PREFER_IF
				T x = *left++;
				if (comp(x, piv)) *lwr++ = x;
				else *rwr-- = x;
#else
				int h = comp(*left, piv);
				*lwr = *rwr = *left++;
				lwr += h; rwr -= !h;
#endif
			}
		}
		while (left - lwr > UNROLL && right - left >= UNROLL) {
			for (int i = UNROLL; i--;) {
#if PREFER_IF
				T x = *right--;
				if (comp(x, piv)) *lwr++ = x;
				else *rwr-- = x;
#else
				int h = comp(*right, piv);
				*rwr = *lwr = *right--;
				rwr -= !h; lwr += h;
#endif
			}
		}
	}
	while ((lwr < left||rwr > right) && left <= right) {
		while (rwr > right && left <= right) {
			T x = *left++;
			if (comp(x, piv)) *lwr++ = x;
			else *rwr-- = x;
		}
		while (lwr < left && left <= right) {
			T x = *right--;
			if (comp(x, piv)) *lwr++ = x;
			else *rwr-- = x;
		}
	}
	std::memcpy(lwr, sw + 1, swsz * sizeof(T));
	*outerleft = *rwr;
	*rwr = piv;
	return rwr;
}

template<typename T, typename Compare>
static void smallsort(T* left, T* right, Compare comp) {
	while (right - left > 15) {
		T* mid = partition_small(left, right, comp);
		if (mid == NULL) return;
		smallsort(left, mid - 1, comp);
		left = mid + 1;
	}
	sorting_network(left, right - left, comp);
}

template<typename T, typename Compare>
static void blqsort(T* left, T* right, Compare comp) {
	int badparts = 0;
	while (1) {
		ptrdiff_t partszm1 = right - left;
		if (partszm1 <= SMALLPART) break;

		T* mid = partition_large(left, right, comp);

		if (mid - left < right - mid) {
			blqsort(left, mid - 1, comp);
			left = mid + 1;
		}
		else {
			blqsort(mid + 1, right, comp);
			right = mid - 1;
		}

		if (right - left > partszm1 / 64 * 63) {
			badparts += 1;
			if (badparts > 32) {
				heap_sort(left, right, comp);
				return;
			}
			if (left == mid + 1) {
				T piv = *mid;
				mid++;
				// collect duplicates
				for (T* p = left; p <= right; p++) {
					if (!comp(piv, *p)) {
						std::swap(*mid, *p);
						mid++;
					}
				}
				left = mid;
				if (right - left < SMALLPART) break;
			}
		}
	}
	smallsort(left, right, comp);
}

template <typename T, typename Compare>
static inline void insert_sort(T* left, T* right, Compare comp) {
	for (T* i = left + 1; i <= right; i++) {
		T key = std::move(*i);
		T* j = i;
		while (j > left && comp(key, *(j - 1))) {
			*j = std::move(*(j - 1));
			j--;
		}
		*j = std::move(key);
	}
}

template<typename T, typename Compare>
static inline void sort2b(T* a, T* b, Compare comp) {
	if (comp(*b, *a)) std::swap(*a, *b);
}

template<typename T, typename Compare>
static inline void med5b(T* a, T* b, T* c, T* d, T* e, Compare comp) {
	sort2b(a, b, comp); sort2b(c, d, comp);
	sort2b(a, c, comp); sort2b(b, d, comp);
	sort2b(b, c, comp); sort2b(c, e, comp);
	sort2b(b, c, comp);
}

template <typename T, typename Compare>
static inline int med3b(T* a, T* b, T* c, Compare comp) {
	int cnt = 0;
	if (comp(*b, *a)) {
		std::swap(*a, *b);
		cnt += 1;
	}
	if (comp(*c, *a)) {
		std::swap(*a, *c);
		cnt += 1;
	}
	if (comp(*c, *b)) {
		std::swap(*b, *c);
		cnt += 1;
	}
	return cnt;
}

// Based on BlockQuicksort by Edelkamp and Weiß

template <typename T, typename Compare>
static void block_qsort(T* left0, T* right0, Compare comp) {

	int badparts = 0;
	while (right0 - left0 > 16) {

		size_t sizem1 = right0 - left0;
		T* mid = left0 + sizem1 / 2;

		T* left = left0 + 1;
		T* right = right0;

		if (sizem1 > 15000) {
			if (sizem1 > 250000) {
				med5b(left, left + 1, left + 2, left + 3, left + 4, comp);
				med5b(mid - 7, mid - 6, mid - 5, mid - 4, mid - 3, comp);
				med5b(mid - 2, mid - 1, mid, mid + 1, mid + 2, comp);
				med5b(mid + 3, mid + 4, mid + 5, mid + 6, mid + 7, comp);
				med5b(right - 4, right - 3, right - 2, right - 1, right, comp);
				med5b(left + 2, mid - 5, mid, mid + 5, right - 2, comp);
			}
			else {
				med3b(left, left + 1, left + 2, comp);
				med3b(mid - 1, mid, mid + 1, comp);
				med3b(right - 2, right - 1, right, comp);
				med3b(left + 1, mid, right - 1, comp);
			}
		}
		else {
			int nsw = med3b(left, mid, right, comp);
			left += 1;
			right -= 1;

			if (nsw == 0) {
				int nwrong = 0;
				for (T* pi = left0 + 1; pi <= right0; pi++) {
					nwrong += comp(*pi, *(pi - 1));
					if (nwrong > 3) goto continue_partition;
				}
				if (nwrong == 0) return;		// already sorted
				//printf("insert-sort\n");
				break;
			}
		}

	continue_partition:

		{
			T piv = std::move(*mid);
			*mid = std::move(*left0);

			T* left_p[BLSZ];
			T* right_p[BLSZ];

			int nleft = 0;
			int nright = 0;
			int left_offs = 0, right_offs = 0;

			int minx = std::min(BLSZ, right - left + 1);
			for (T* endl = left + minx; left < endl; left++) {
				left_p[nleft] = left;
				nleft += !comp(*left, piv);
			}

			// 99.9% pre sorted -> save 1%
			if (minx == BLSZ && nleft <= 2) {
				// Hoare
				if (nleft != 0) left = left0 + 1;
				while (1) {
					while (left <= right && comp(*left, piv)) left++;
					while (left <= right && comp(piv, *right)) right--;
					if (left >= right) break;
					std::iter_swap(left, right);
					left++;
					right--;
				}
				goto part_done;
			}

			while (1) {
				int nswaps = std::min(nleft, nright);
				for (int k = 0; k < nswaps; k++) {
					std::iter_swap(left_p[left_offs], right_p[right_offs]);
					left_offs++;
					right_offs++;
				}
				nleft -= nswaps;
				nright -= nswaps;

				if (right < left) break;

				int blsz = std::min(BLSZ, right - left + 1);
				if (nleft == 0) {
					for (T* endl = left + blsz; left < endl; left++) {
						left_p[nleft] = left;
						nleft += !comp(*left, piv);
					}
					left_offs = 0;
				}
				else {
					for (T* endr = right - blsz; right > endr; right--) {
						right_p[nright] = right;
						nright += !comp(piv, *right);
					}
					right_offs = 0;
				}
			}
			if (nleft) {
				for (int k = left_offs + nleft - 1; k >= left_offs; k--, right--) {
					std::iter_swap(left_p[k], right);
				}
			}
			else {
				for (int k = right_offs + nright - 1; k >= right_offs; k--, left++) {
					std::iter_swap(right_p[k], left);
				}
				right = left - 1;
			}
		part_done:
			*left0 = std::move(*right);
			*right = std::move(piv);
		}

		if (right - left0 < right0 - right) {
			block_qsort(left0, right - 1, comp);
			left0 = right + 1;
		} else {
			block_qsort(right + 1, right0, comp);
			right0 = right - 1;
		}
		if (right0 - left0 > sizem1 / 64 * 63 + 4) {
			badparts += 1;
			if (badparts > 32) {
				// printf("heapsort\n");
				heap_sort(left0, right0, comp);
				return;
			}
		}

	}
	insert_sort(left0, right0, comp);
}

template <typename T, typename Compare = std::less<T>>
void sort(T* first, T* last, Compare comp = Compare()) {

	if (last - first < 2) return;
	for (T* p = first + 1; p < last; p++) {
		if (comp(*p, *(p - 1))) goto not_asc_sorted;
	}
	return;
not_asc_sorted:
	for (T* p = last - 1; p > first; p--) {
		if (comp(*(p - 1), *p)) goto not_desc_sorted;
	}
	for (ptrdiff_t i = 0; i < (last - first) / 2; i++) {
		std::swap(first[i], last[-i - 1]);
	}
	return;
not_desc_sorted:

	constexpr bool cheap =
		std::is_trivially_copyable_v<T> &&
		std::is_trivially_default_constructible_v<T> &&
		sizeof(T) <= 16;

	if constexpr (cheap) {
		blqsort(first, last - 1, comp);
	}
	else {
		block_qsort(first, last - 1, comp);
	}
}
}
#endif
