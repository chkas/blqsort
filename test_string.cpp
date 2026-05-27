#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

#include <algorithm>
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
double cputime() {
	return (double)clock() / CLOCKS_PER_SEC;
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

	t0 = cputime();
	blqs::sort(data, data + SIZE);
	printf("blqs::sort %.3fs\n", cputime() - t0);
	test();
}
