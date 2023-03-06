//currently just a copy of blocksworld

#include "hanoi.hpp"
#include "../search/main.hpp"
#include <cstdio>

int main(int argc, const char *argv[]) {
	dfheader(stdout);
	Hanoi d(stdin);
  search<Hanoi>(d, argc, argv);
	dffooter(stdout);
	return 0;
}
