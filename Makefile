BUFLEN ?= 1000000000
CFLAGS  = -D BUFLEN=${BUFLEN} -std=c99 -pedantic -pedantic-errors -Wall -O0

benchmark: nop.c benchmark.c
	${CC} ${CFLAGS} -c -o benchmark.o benchmark.c
	${CC} ${CFLAGS} -c -o nop.o       nop.c
	${CC} ${CFLAGS}    -o benchmark   benchmark.o nop.o

clean:
	rm -f *.o benchmark

.PHONY: clean
