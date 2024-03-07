/*
	Compare the speed of reading an array randomly vs linearly.

	Filename:   random_access.c
	Compile:    cc -D BUFLEN=1000000000 -O0 -s -o random_access random_access.c
	Run:        ./random_access > /dev/null
 */

#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>

#include <dlfcn.h>

#ifndef BUFLEN
#define BUFLEN (1000000000)
#endif

void (*nop)(char);

int
main(int argc, char** argv)
{
	clock_t start, end;
	void* handle;

	char* buf = malloc(BUFLEN);
	uint32_t* ibuf = malloc(BUFLEN * sizeof(uint32_t));
	assert(buf && ibuf);

	memset(buf, 'A', BUFLEN);

	// Load nop() from nop.so
	handle = dlopen("./nop.so", RTLD_LAZY);
	assert(handle);
	nop = dlsym(handle, "nop");
	assert(!dlerror());

	{ // Prep Linear Index
		start = clock();
		for (size_t i = 0; i < BUFLEN; ++i) {
			ibuf[i] = i;
		}
		end = clock();
		fprintf(stderr, "Building Linear Index: %5d sec\n", (end - start) / CLOCKS_PER_SEC);
	}

	{ // Linear access
		start = clock();
		for (size_t i = 0; i < BUFLEN; ++i) {
			char x = buf[ibuf[i]];
			nop(x); // Prevents optimizations
		}
		end = clock();
		fprintf(stderr, "Linear Access:         %5d sec\n", (end - start) / CLOCKS_PER_SEC);
	}

	{ // Prep Random Index (by scrambling linear index)
		srand(0);
		start = clock();
		for (size_t i = 0; i < BUFLEN; ++i) {
			uint32_t n = ibuf[i];
			ibuf[i] = ibuf[(size_t)rand() % BUFLEN];
			ibuf[ibuf[i]] = n;
		}
		end = clock();
		fprintf(stderr, "Building Random Index: %5d sec\n", (end - start) / CLOCKS_PER_SEC);
	}

	{ // Random access
		start = clock();
		for (size_t i = 0; i < BUFLEN; ++i) {
			char x = buf[ibuf[i]];
			nop(x); // Prevents optimizations
		}
		end = clock();
		fprintf(stderr, "Random Access:         %5d sec\n", (end - start) / CLOCKS_PER_SEC);
	}

	dlclose(handle);

	return 0;
}