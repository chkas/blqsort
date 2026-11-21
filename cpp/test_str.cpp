// SPDX-License-Identifier: MIT
#include <cstdio>
#include <cstdlib>
#include <chrono>
uint32_t state = 1;
void mrand_init() {
	state = 1;
}
uint32_t mrand(void) {
	uint32_t x = state;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	state = x;
	return x;
}

#include <algorithm>
#include "blqs.h"

constexpr int SIZE = 10000000;

std::string data_arr[SIZE];
std::string data_arr2[SIZE];
std::string* data = data_arr;
std::string* data2 = data_arr2;

void shuffle(int pre_sort) {
	for (int i = 0; i < SIZE; i++) data2[i] = "";
	for (int i = 0; i < SIZE / pre_sort; i++) {
		std::string val;
		int r;
		do r = rand() % SIZE; while (data[r] == "");
		val = std::move(data[r]);
		data[r] = "";
		do r = rand() % SIZE; while (data2[r] != "");
		data2[r] = std::move(val);
	}
	for (int i = 0, j = 0; i < SIZE; i++) {
		while (data2[j] != "") j++;
		if (data[i] != "") data2[j] = std::move(data[i]);
	}
	std::swap(data, data2);
}
void xshuffle_swp(int pre_sort) {
	// simpler, but not realistic
	for (int i = 0; i < SIZE / pre_sort / 2; i++) {
		std::swap(data[rand() % SIZE], data[rand() % SIZE]);
	}
}
void chksum_test(uint32_t expect) {
    uint32_t hash = 2166136261u;

	for (size_t i = 0; i < SIZE; ++i) {
    	const auto& s = data[i];
        for (unsigned char c : s) {
            hash ^= c;
            hash *= 16777619u;
        }
        hash ^= 0;
        hash *= 16777619u;
    }
	if (hash != expect) printf("Checksum error 0x%x\n", hash);
}
double time() {
	static auto time0 = std::chrono::high_resolution_clock::now();
	auto t = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> diff = t - time0;
	return diff.count();
}

#define STRSZ 10
std::string genstr() {
  std::string ret = "";
  for (int i = 0; i < STRSZ; ++i) ret += mrand() % 26 + 'a';
  return ret;
}

int main() {
	double t0;

	printf("blqsort - sorting %d million strings ...\n", SIZE / 1000000);
	for (int i = 0; i < SIZE; i++) data[i] = genstr();
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("Random: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);
//return 0;

	shuffle(10);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("90%% sorted: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);
	
	shuffle(100);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("99%% sorted: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);

	shuffle(1000);
	t0 = time();
	blqs::sort(data, data + SIZE);
	printf("99.9%% sorted: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);

	printf("\n");
//return 0;
// -------------------------------------------------------------

	mrand_init();
	printf("std::sort - sorting %d million strings ...\n", SIZE / 1000000);
	for (int i = 0; i < SIZE; i++) data[i] = genstr();
	t0 = time();
	std::sort(data, data + SIZE);
	printf("Random: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);

	shuffle(10);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("90%% sorted: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);
	
	shuffle(100);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("99%% sorted: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);

	shuffle(1000);
	t0 = time();
	std::sort(data, data + SIZE);
	printf("99.9%% sorted: %.2fs\n", time() - t0);
	chksum_test(0xf3d21360);
	
	printf("\n");
}
