## blqsort

**blqsort** is a fast branchless quicksort implementation for C++ that is typically faster than `std::sort` and `pdqsort`.

On modern CPUs, **avoiding branch misprediction** is a key technique to speed up programs: <a href="https://easylang.online/blog/branchless">When 'if' slows you down, avoid it.</a>

Performance results naturally depend on the underlying hardware. The following benchmarks show the execution times for sorting 50 million `doubles` using different sorting implementations. The measurements were taken on an *Apple M1* system using *Clang* and on an *AMD Ryzen 3* system using *GCC*, both compiled with the `-O3` option.

| Implementation | Apple M1 | AMD Ryzen |
| :--- | :--- | :--- |
| std::sort | 1.33s | 5.56s |
| pdqsort | 1.33s | 2.81s |
| blqsort | 1.01s | 2.06s |

[This paper](https://arxiv.org/abs/1604.06697) by *Edelkamp* and *A. Weiß* shows how partitioning performance in Quicksort can be improved by avoiding conditional branches.

The strategy of using an auxiliary buffer for **branchless partitioning** is inspired by [fluxsort](https://github.com/scandum/fluxsort). The “auxiliary buffer” here means a  512‑element stack array, not heap memory.

To avoid the `O(n²)` runtime caused by bad input data, the program can group identical elements together and switch to *heapsort* for that specific part if it detects a big imbalance during partitioning. The program also checks if a partition is already sorted.

For larger parts, it uses a median-of-medians strategy to find a good pivot. In addition, critical partitioning loops are explicitly unrolled.

For 2 to 12 elements, the algorithm uses custom sorting networks. This approach requires a separate code path for each size but sorts small subsets with very few swaps using a branchless *sort‑2* primitive. [Source for sorting networks](https://bertdobbelaere.github.io/sorting_networks.html)

As a result, **blqsort** becomes faster than, for example, *std::sort* and *pdqsort* for random numbers.

For types with higher copy or move costs (such as strings), the buffer-based branchless approach becomes less efficient. In this case, a **BlockQuicksort** variant is used, where only element indices are processed branchlessly, and the actual data is moved using fewer swap operations.

## Usage

You only need to include `blqsort.h`, and it can be used just as easily as *std::sort*.

~~~
.
#include "blqs.h"
double data[SIZE];
	.
	blqs::sort(data, data + SIZE);
	.
~~~

### Sorting Custom Data Structures

In practice, we often need to sort custom data structures. This is where *SIMD* libraries like *Google Highway* - while very fast for simple numbers - become difficult to use.

Using *std::sort* or *blqs::sort* gives you much more flexibility;

~~~
.
struct entry {
    int32_t id;
    int32_t value;
    bool operator<(const entry& other) const {
        return id < other.id;
    }
};
struct entry data[SIZE];

	.
	blqs::sort(data, data + SIZE);
  	.
~~~

Execution times for sorting 50 million of this `structs`.

| Implementation | Apple M1 | AMD Ryzen |
| :--- | :--- | :--- |
| std::sort| 3.46s | 4.75s |
| pdqsort | 3.46s | 4.72s |
| blqsort | 0.97s | 2.20s |




