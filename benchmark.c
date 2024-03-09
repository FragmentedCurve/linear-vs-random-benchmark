/*
	Compare the speed of reading an array randomly vs linearly.
 */

#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

#ifndef BUFLEN
#define BUFLEN (1000000000)
#endif

void nop(char);

int
main(int argc, char** argv)
{
	clock_t start, end;

	int       time[2] = {0};
	char*     buf     = malloc(BUFLEN);
	uint32_t* ibuf    = malloc(BUFLEN * sizeof(uint32_t));

	assert(buf && ibuf && "Memory allocation failed.");
	memset(buf, 'A', BUFLEN);

	{ // Prep Linear Index
		for (size_t i = 0; i < BUFLEN; ++i) {
			ibuf[i] = i;
		}
	}

	{ // Linear access
		start = clock();
		for (size_t i = 0; i < BUFLEN; ++i) {
			char x = buf[ibuf[i]];
			nop(x); // Prevents optimizations
		}
		end = clock();
		time[0] = (end - start) / CLOCKS_PER_SEC;
	}

	{ // Prep Random Index (by scrambling linear index)
		srand(0);
		for (size_t i = 0; i < BUFLEN; ++i) {
			uint32_t n = ibuf[i];
			ibuf[i] = ibuf[(size_t)rand() % BUFLEN];
			ibuf[ibuf[i]] = n;
		}
	}

	{ // Random access
		start = clock();
		for (size_t i = 0; i < BUFLEN; ++i) {
			char x = buf[ibuf[i]];
			nop(x); // Prevents optimizations
		}
		end = clock();
		time[1] = (end - start) / CLOCKS_PER_SEC;
	}

	printf("%10s   %10s\n", "Linear (s)", "Random (s)");
	printf("%10d   %10d\n", time[0], time[1]);

	return 0;
}
