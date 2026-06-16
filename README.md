## blqsort - fast branchless quicksort

There are four implementations of **blqsort** here, each provided as a single header file.

| File | Description |
| :--- | :--- |
| [blqs.h](cpp/blqs.h) | C++ Single-Threaded |
| [blqs_thr.h](cpp/blqs_thr.h) | C++ Multi-Threaded |
| [blqsort.h](c/blqsort.h) | C Single-Threaded |
| [blqsort_thr.h](c/blqsort_thr.h) | C Multi-Threaded |

Performance results naturally depend on the underlying hardware. The following benchmarks
show the execution times for sorting 50 million `doubles` using different sorting
implementations. The measurements were taken on an *Apple M1* system using *Clang*
and on an *Intel i5-8400 Linux* system using *GCC*, both compiled with the `-O3`
option. [test_double.cpp](cpp/test_double.cpp)

| Implementation | Apple M1 | Intel i5-8400n |
| :--- | :--- | :--- |
| std::sort | 1.33s | 3,97s |
| **blqsort** | **0.66s** | 1.29s |

For a fair comparison, the single-threaded version of `blqs` was used here. On an M1, the threaded versions are another factor of 3 faster. In terms of runtime, the C++ versions differ only very little from the C version.

---

There are now different code paths: one for the branchless variant and one for the `if` variant, which on macOS compiles to efficient CSEL instructions.

In the *blqs* header, it is currently defined as:

~~~
#if defined(__arm64__)
	#define PREFER_IF 1
#else
	#define PREFER_IF 0
#endif
~~~

and used in the code like this:

~~~
#if PREFER_IF
		T x = *left++;
		if (comp(x, piv)) *lwr++ = x;
		else *sw++ = x;
#else
		int h = comp(*left, piv);
		*lwr = *sw = *left++;
		lwr += h; sw += !h;
#endif
~~~

Since *Clang* on Linux also generates efficient code for the `if`-version, the following variant also might make sense:

~~~
#if defined(__GNUC__) && !defined(__clang__)
	#define PREFER_IF 0
#else
	#define PREFER_IF 1
#endif
~~~

---

[This paper](https://arxiv.org/abs/1604.06697) by *Edelkamp* and *Weiß* shows how partitioning performance in Quicksort can be improved by avoiding conditional branches.

The strategy of using an auxiliary buffer for **branchless partitioning** is inspired by
[fluxsort](https://github.com/scandum/fluxsort). The “auxiliary buffer” here means a
1024‑element stack array, not heap memory.

First, 1024 elements are copied from one side into an auxiliary buffer to make room for
subsequent operations. Then, we alternately copy a 1024-element block to the left and right in a branchless manner. The left pointer is only incremented if the element is smaller than the pivot, otherwise, the right pointer is incremented - branchless, of course.

This involves more more than double the necessary copy operations. For data types that are cheap to copy, however, this is much less expensive than the branch mispredictions that would
otherwise occur.

### Pivot strategy, bad input and sorting network

To avoid the `O(n²)` runtime caused by bad input data, the program can group identical elements together and switch to *heapsort* for that specific part if it detects a big imbalance during partitioning. The program also checks if a partition is already sorted.

For larger parts, it uses a median-of-medians strategy to find a good pivot. In addition,
critical partitioning loops are explicitly unrolled.

For 2 to 16 elements, the algorithm uses custom sorting networks. This approach requires a
separate code path for each size but sorts small subsets with very few swaps using a branchless *sort-2* primitive. [Source for sorting networks](https://bertdobbelaere.github.io/
sorting_networks.html)

## C++

For types with higher copy costs that are not `is_trivially_copyable` (such as strings), the
buffer-based branchless approach becomes less efficient. In such cases, a **BlockQuicksort**
variant is used instead. This processes only the element indices in a branchless manner and then moves the actual data with fewer swaps. Some ideas are from
[pdqsort](https://github.com/orlp/pdqsort).

### Usage

You only need to include `blqs.h`, and it can be used just as easily as *std::sort*.

```cpp
#include "blqs.h"

double data[SIZE];

blqs::sort(data, data + SIZE);
```

For the C++ multithreaded variant, which uses C++ threads, include `blqs_thr.h` instead of
`blqs.h`. The function call remains the same.

## C

In C, the code specialized for the data type is generated using `#define`.

### Usage

```c
#define BLQS_CMP(a, b) ((a) < (b))
#define BLQS_TYPE double
#include "blqsort.h"

double data[SIZE];

blqsort(data, SIZE);
```

For the C multithreaded variant, which uses POSIX threads, include `blqsort_thr.h` instead of
`blqsort.h`. The function call remains the same here as well.

## Sorting Custom Data Structures

In practice, we often need to sort custom data structures. This is where *SIMD* libraries like *Google Highway* - while very fast for simple numbers - become difficult to use.

Using *std::sort* or *blqsort* gives you much more flexibility.

### C++

```cpp
#include "blqs.h"

struct entry {
    int id;
    int value;

    bool operator<(const entry& other) const {
        return id < other.id;
    }
};

struct entry data[SIZE];

blqs::sort(data, data + SIZE);
```

### C

```c
struct entry {
	int id;
	int value;
};
#define BLQS_CMP(a, b) (((a).id) < ((b).id))
#define BLQS_TYPE struct entry
#include "blqsort.h"

struct entry data[SIZE];

blqsort(data, SIZE);
```

Execution times for sorting 50 million of these `structs`.

| Implementation | Apple M1 | Intel i5 |
| :--- | :--- | :--- |
| std::sort | 3.46s | 4.10s |
| **blqsort** | 0.76s | 1.40s |

### Links

[When 'if' slows you down, avoid it.](https://easylang.online/blog/branchless)

[Interactive sorting demo](https://easylang.online/sorting)
