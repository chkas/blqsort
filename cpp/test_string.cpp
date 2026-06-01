// SPDX-License-Identifier: MIT
#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <string>

#include "blqs.h"

constexpr int SIZE = 5000000;
std::string data[SIZE];

void test() {
	for (int i = 1; i < SIZE; i++) {
		if (data[i] < data[i - 1]) {
			printf("ERROR ORDER\n");
			return;
		}
	}
}
static auto time0 = std::chrono::high_resolution_clock::now();
double time() {
    auto t = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = t - time0;
    return diff.count();
}

#define STRSZ 40
std::string genstr() {
  std::string ret = "";
  for (int i = 0; i < STRSZ; ++i) ret += rand() % 26 + 'a';
  return ret;
}
int main(int argc, char* argv[]) {

	double t0;
	printf("Sorting %d million strings with length %d ...\n", SIZE / 1000000, STRSZ);
	for (int i = 0; i < SIZE; i++) data[i] = genstr();

	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("blqs::sort %.3fs\n", time() - t0);
	test();
}
