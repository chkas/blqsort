// SPDX-License-Identifier: MIT
// blqs.h - Threaded Branchless Quicksort
// (c) 2026 christof.kaser@gmail.com

#ifndef BLQS_THR_H
#define BLQS_THR_H

#include <cstddef>
#include <cstring>
#include <type_traits>
#include <functional>
#include <utility>

#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <condition_variable>

namespace blqs {

constexpr int SMALLPART = 512;
constexpr int SWSZ = 1024;
constexpr int UNROLL = 16;

constexpr int BLSZ = 512;

template<typename T, typename Compare>
static inline void sort2(T& a, T& b, Compare comp) {
	T x = a;
	T y = b;
	bool m = comp(x, y);
	a = m ? x : y;
	b = m ? y : x;
}

template<typename T, typename Compare>
static inline void sort3(T& a, T& b, T& c, Compare comp) {
	sort2(a, b, comp);
	sort2(b, c, comp);
	sort2(a, b, comp);
}

template<typename T, typename Compare>
static inline void sort4(T& a, T& b, T& c, T& d, Compare comp) {
	sort2(a, b, comp);
	sort2(c, d, comp);
	sort2(a, c, comp);
	sort2(b, d, comp);
	sort2(b, c, comp);
}

template<typename T, typename Compare>
static inline void sort5(T& a, T& b, T& c, T& d, T& e, Compare comp) {
	sort2(b, c, comp);
	sort2(d, e, comp);
	sort2(b, d, comp);
	sort2(a, c, comp);
	sort2(a, d, comp);
	sort2(c, e, comp);
	sort2(a, b, comp);
	sort2(c, d, comp);
	sort2(b, c, comp);
}

template<typename T, typename Compare>
static inline void sort6(T& a, T& b, T& c, T& d, T& e, T& f, Compare comp) {
	sort2(a, b, comp);
	sort2(c, d, comp);
	sort2(e, f, comp);
	sort2(a, c, comp);
	sort2(b, d, comp);
	sort2(e, f, comp);
	sort2(a, e, comp);
	sort2(b, f, comp);
	sort2(c, e, comp);
	sort2(d, f, comp);
	sort2(b, c, comp);
	sort2(d, e, comp);
	sort2(c, d, comp);
}

template<typename T, typename Compare>
static inline void sort7(T& a, T& b, T& c, T& d, T& e, T& f, T& g, Compare comp) {
	sort2(a, b, comp);
	sort2(c, d, comp);
	sort2(a, c, comp);
	sort2(b, d, comp);
	sort2(b, c, comp);
	sort2(e, f, comp);
	sort2(e, g, comp);
	sort2(f, g, comp);
	sort2(a, e, comp);
	sort2(b, f, comp);
	sort2(c, g, comp);
	sort2(b, e, comp);
	sort2(d, g, comp);
	sort2(c, e, comp);
	sort2(b, c, comp);
	sort2(d, f, comp);
	sort2(d, e, comp);
}

template<typename T, typename Compare>
static inline void sort8(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, Compare comp) {
	sort7(a, b, c, d, e, f, g, comp);
	sort2(g, h, comp);
	sort2(f, g, comp);
	sort2(e, f, comp);
	sort2(d, e, comp);
	sort2(c, d, comp);
	sort2(b, c, comp);
	sort2(a, b, comp);
}

template<typename T, typename Compare>
static inline void sort9(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, Compare comp) {
	sort8(a, b, c, d, e, f, g, h, comp);
	sort2(h, i, comp);
	sort2(g, h, comp);
	sort2(f, g, comp);
	sort2(e, f, comp);
	sort2(d, e, comp);
	sort2(c, d, comp);
	sort2(b, c, comp);
	sort2(a, b, comp);
}

template<typename T, typename Compare>
static inline void sort10(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, Compare comp) {
	sort9(a, b, c, d, e, f, g, h, i, comp);
	sort2(i, j, comp);
	sort2(h, i, comp);
	sort2(g, h, comp);
	sort2(f, g, comp);
	sort2(e, f, comp);
	sort2(d, e, comp);
	sort2(c, d, comp);
	sort2(b, c, comp);
	sort2(a, b, comp);
}

template<typename T, typename Compare>
static inline void sort11(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k, Compare comp) {
	sort10(a, b, c, d, e, f, g, h, i, j, comp);
	sort2(j, k, comp);
	sort2(i, j, comp);
	sort2(h, i, comp);
	sort2(g, h, comp);
	sort2(f, g, comp);
	sort2(e, f, comp);
	sort2(d, e, comp);
	sort2(c, d, comp);
	sort2(b, c, comp);
	sort2(a, b, comp);
}

template<typename T, typename Compare>
static inline void sort12(T& a, T& b, T& c, T& d, T& e, T& f, T& g, T& h, T& i, T& j, T& k, T& l, Compare comp) {
	sort11(a, b, c, d, e, f, g, h, i, j, k, comp);
	sort2(k, l, comp);
	sort2(j, k, comp);
	sort2(i, j, comp);
	sort2(h, i, comp);
	sort2(g, h, comp);
	sort2(f, g, comp);
	sort2(e, f, comp);
	sort2(d, e, comp);
	sort2(c, d, comp);
	sort2(b, c, comp);
	sort2(a, b, comp);
}

template<typename T, typename Compare>
static inline void sorting_network(T* left, int partsz_min_1, Compare comp) {
	switch (partsz_min_1) {
	case 11:
		sort12(left[0], left[1], left[2], left[3], left[4], left[5],
			   left[6], left[7], left[8], left[9], left[10], left[11], comp);
		break;
	case 10:
		sort11(left[0], left[1], left[2], left[3], left[4], left[5],
			   left[6], left[7], left[8], left[9], left[10], comp);
		break;
	case 9:
		sort10(left[0], left[1], left[2], left[3], left[4],
			   left[5], left[6], left[7], left[8], left[9], comp);
		break;
	case 8:
		sort9(left[0], left[1], left[2], left[3], left[4],
			  left[5], left[6], left[7], left[8], comp);
		break;
	case 7:
		sort8(left[0], left[1], left[2], left[3],
			  left[4], left[5], left[6], left[7], comp);
		break;
	case 6:
		sort7(left[0], left[1], left[2], left[3],
			  left[4], left[5], left[6], comp);
		break;
	case 5:
		sort6(left[0], left[1], left[2], left[3],
			  left[4], left[5], comp);
		break;
	case 4:
		sort5(left[0], left[1], left[2], left[3], left[4], comp);
		break;
	case 3:
		sort4(left[0], left[1], left[2], left[3], comp);
		break;
	case 2:
		sort3(left[0], left[1], left[2], comp);
		break;
	case 1:
		sort2(left[0], left[1], comp);
		break;
	default:
		break;
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
void heap_sort(T* left, T* right, Compare comp) {
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
	sort2(a, b, comp);
	sort2(c, d, comp);
	sort2(a, c, comp);
	sort2(b, d, comp);
	sort2(b, c, comp);
	sort2(c, e, comp);
	sort2(b, c, comp);
}

template<typename T, typename Compare>
static T* partition_small(T* left, T* right, Compare comp) {
	T* outerleft = left;
	T* pivp = left + (right - left) / 2;

	T l1 = left[1], l2 = left[2];
	T piv = *pivp;
	T r1 = right[-1], r0 = *right;
	med5(l1, l2, piv, r1, r0, comp);
	left[1] = l1; left[2] = l2;
	right[-1] = r1; *right = r0;

	left += 3; right -= 2;
	*pivp = *outerleft;

	T swbuf[SMALLPART];
	T *sw = swbuf, *lwr = left;
	while (left <= right) {
		bool h = comp(*left, piv);
		*lwr = *sw = *left++;
		lwr += h; sw += !h;
	}
	std::move(swbuf, sw, lwr);
	lwr -= 1;
	*outerleft = *lwr;
	*lwr = piv;
	return lwr;
}

template<typename T, typename Compare>
static T* partition_large(T* left, T* right, Compare comp) {
	T* outerleft = left;
	T* pivp = left + (right - left) / 2;

	T piv = *pivp;

	med5(left[1], left[2], left[3], left[4], left[5], comp);
	med5(left[21], left[22], left[23], left[24], left[25], comp);
	med5(pivp[-2], pivp[-1], piv, pivp[1], pivp[2], comp);
	med5(right[-14], right[-13], right[-12], right[-11], right[-10], comp);
	med5(right[-4], right[-3], right[-2], right[-1], right[0], comp);
	med5(left[3], left[23], piv, right[-12], right[-2], comp);

	left += 1;
	*pivp = *outerleft;

	while (comp(*left, piv)) left++;
	if (left >= outerleft + 32) {
		// could be sorted
		*pivp = piv;
		for (T* p = outerleft + 1; p <= right; p++) {
			if (comp(*p, *(p - 1))) {
				*pivp = *outerleft;
				goto not_sorted;
			}
		}
		return NULL;
	}
not_sorted:
	while (comp(piv, *right)) right--;

	T swbuf[SWSZ];
	T *lwr = left, *rwr = right, *sw = swbuf;

	while (sw < swbuf + SWSZ - UNROLL && left <= right - UNROLL) {
		for (int i = UNROLL; i--;) {
			bool h = comp(*right, piv); *rwr = *sw = *right--; rwr -= !h; sw += h;
		}
	}
	while (sw < swbuf + SWSZ - UNROLL && left <= right) {
		bool h = comp(*right, piv); *rwr = *sw = *right--; rwr -= !h; sw += h;
	}
	while (left <= right - UNROLL) {
		while (rwr > right + UNROLL && left <= right - UNROLL) {
			for (int i = UNROLL; i--;) {
				bool h = comp(*left, piv); *lwr = *rwr = *left++; lwr += h; rwr -= !h;
			}
		}
		while (lwr < left - UNROLL && left <= right - UNROLL) {
			for (int i = UNROLL; i--;) {
				bool h = comp(*right, piv); *rwr = *lwr = *right--; rwr -= !h; lwr += h;
			}
		}
	}
	while (rwr > right && left <= right) {
		bool h = comp(*left, piv); *lwr = *rwr = *left++; lwr += h; rwr -= !h;
	}
	while (left <= right) {
		bool h = comp(*right, piv); *rwr = *lwr = *right--; rwr -= !h; lwr += h;
	}
	std::move(swbuf, sw, lwr);
	*outerleft = *rwr;
	*rwr = piv;
	return rwr;
}

// ------------------------------------------------------------
// for non-trivial copyable path
// ------------------------------------------------------------

template<typename T, typename Compare>
static inline void insert_sort(T* left, T* right, Compare comp) {
	if (left >= right) return;

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
static inline void med3(T* a, T* b, T* c, Compare comp) {
	if (comp(*b, *a)) std::iter_swap(a, b);
	if (comp(*c, *a)) std::iter_swap(a, c);
	if (comp(*c, *b)) std::iter_swap(b, c);
}

// ------------------------------------------------------------
// threads
// ------------------------------------------------------------

constexpr ptrdiff_t THREAD_MIN_TRIVIAL = 1000000;
constexpr ptrdiff_t THREAD_MIN_OBJECT  = 250000;

template<typename T, typename Compare>
static inline void blqsort(T* left, T* right, Compare comp);

template<typename T, typename Compare>
static inline void sort_thr(T* left, T* right, Compare comp);

template<typename T, typename Compare>
static inline void block_qsort(T* left0, T* right0, Compare comp);

template<typename T, typename Compare>
static inline void block_sort_thr(T* left, T* right, Compare comp);

static inline unsigned max_threads;
static inline std::atomic<int> n_threads{0};

static inline std::mutex mtx;
static inline std::condition_variable cond;


template<typename T, typename Compare>
static inline void sort_thr(T* left, T* right, Compare comp) {
	blqsort(left, right, comp);

	std::lock_guard<std::mutex> lock(mtx);
	if (--n_threads == 0) cond.notify_one();
}

template<typename T, typename Compare>
static inline void blqsort(T* left, T* right, Compare comp) {
	while (1) {
		ptrdiff_t partszm1 = right - left;
		T* mid;

		if (partszm1 <= SMALLPART) {
			if (partszm1 <= 11) {
				sorting_network(left, (int)partszm1, comp);
				return;
			}
			mid = partition_small(left, right, comp);
		}
		else {
			mid = partition_large(left, right, comp);
			if (mid == NULL) return; // already sortiert

			if ((mid - left) * 16 < partszm1) {
				blqsort(left, mid - 1, comp);

				T piv = *mid;
				mid += 1;
				// collect duplicates
				for (T* p = mid; p <= right; p++) {
					if (!comp(piv, *p)) {
						std::swap(*mid, *p);
						mid++;
					}
				}
				left = mid;
				if (right - left < SMALLPART) {
					blqsort(left, right, comp);
					return;
				}
				// second chance before fallback to heapsort
				mid = partition_large(left, right, comp);
				if ((mid - left) * 16 < partszm1) {
					heap_sort(left, mid - 1, comp);
					heap_sort(mid + 1, right, comp);
					return;
				}
			}
		}


		if ((mid - left) > THREAD_MIN_TRIVIAL && n_threads < (int)max_threads) {
			n_threads++;

			try {
				std::thread(sort_thr<T, Compare>, left, mid - 1, comp).detach();
				left = mid + 1;
				continue;
			}
			catch (...) {
				n_threads--;
			}
		}

		if (mid - left < right - mid) {
			blqsort(left, mid - 1, comp);
			left = mid + 1;
		}
		else {
			blqsort(mid + 1, right, comp);
			right = mid - 1;
		}
	}
}

template<typename T, typename Compare>
static inline void block_qsort(T* left0, T* right0, Compare comp) {
	if (left0 >= right0) return;

	while (right0 - left0 > 16) {
		ptrdiff_t sizem1 = right0 - left0;

		T* mid = left0 + sizem1 / 2;

		T* left = left0 + 1;
		T* right = right0;

		if (sizem1 > 64) {
			med3(left, left + 1, left + 2, comp);
			med3(mid - 1, mid, mid + 1, comp);
			med3(right - 2, right - 1, right, comp);
			med3(left + 1, mid, right - 1, comp);
		}
		else {
			med3(left, mid, right, comp);
			left += 1;
			right -= 1;
		}

		T pivot = std::move(*mid);
		*mid = std::move(*left0);

		T* left_p[BLSZ];
		T* right_p[BLSZ];

		int nleft = 0;
		int nright = 0;
		int left_offs = 0;
		int right_offs = 0;

		while (right >= left) {
			int blsz = (int)std::min<ptrdiff_t>(BLSZ, right - left + 1);

			if (nleft == 0) {
				for (T* endl = left + blsz; left < endl; left++) {
					left_p[nleft] = left;
					nleft += !comp(*left, pivot);
				}
				left_offs = 0;
			}
			else {
				for (T* endr = right - blsz; right > endr; right--) {
					right_p[nright] = right;
					nright += !comp(pivot, *right);
				}
				right_offs = 0;
			}

			int nswaps = nleft < nright ? nleft : nright;

			for (int k = 0; k < nswaps; k++) {
				std::iter_swap(left_p[left_offs], right_p[right_offs]);
				left_offs++;
				right_offs++;
			}

			nleft -= nswaps;
			nright -= nswaps;
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

		*left0 = std::move(*right);
		*right = std::move(pivot);

		ptrdiff_t szl = right - left0;
		ptrdiff_t szr = right0 - right;
		ptrdiff_t szmin = szl < szr ? szl : szr;

		if (szmin * 16 < right0 - left0) {
			heap_sort(left0, right - 1, comp);
			heap_sort(right + 1, right0, comp);
			return;
		}

		if (szl > THREAD_MIN_OBJECT && n_threads < (int)max_threads) {
			n_threads++;

			try {
				std::thread(block_sort_thr<T, Compare>, left0, right - 1, comp).detach();
				left0 = right + 1;
				continue;
			}
			catch (...) {
				n_threads--;
			}
		}

		if (szr > THREAD_MIN_OBJECT && n_threads < (int)max_threads) {
			n_threads++;

			try {
				std::thread(block_sort_thr<T, Compare>, right + 1, right0, comp).detach();
				right0 = right - 1;
				continue;
			}
			catch (...) {
				n_threads--;
			}
		}

		if (szl < szr) {
			block_qsort(left0, right - 1, comp);
			left0 = right + 1;
		}
		else {
			block_qsort(right + 1, right0, comp);
			right0 = right - 1;
		}
	}

	insert_sort(left0, right0, comp);
}

template<typename T, typename Compare>
static inline void block_sort_thr(T* left, T* right, Compare comp) {
	block_qsort(left, right, comp);

	std::lock_guard<std::mutex> lock(mtx);
	if (--n_threads == 0) cond.notify_one();
}

template<typename T, typename Compare = std::less<T>>
inline void sort(T* begin, T* end, Compare comp = Compare()) {
	if (end - begin < 2) return;

	unsigned hw = std::thread::hardware_concurrency();
	max_threads = hw > 0 ? hw * 2 : 8;
	n_threads = 1;

	constexpr bool cheap =
		std::is_trivially_copyable_v<T> &&
		sizeof(T) <= 16 &&
		std::is_default_constructible_v<T>;

	if constexpr (cheap) {
		try {
			std::thread(sort_thr<T, Compare>, begin, end - 1, comp).detach();
		}
		catch (...) {
			sort_thr(begin, end - 1, comp);
		}
	}
	else {
		try {
			std::thread(block_sort_thr<T, Compare>, begin, end - 1, comp).detach();
		}
		catch (...) {
			block_sort_thr(begin, end - 1, comp);
		}
	}

	std::unique_lock<std::mutex> lock(mtx);
	cond.wait(lock, [] {
		return n_threads == 0;
	});

}
}

#endif
