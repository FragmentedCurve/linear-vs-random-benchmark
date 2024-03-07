benchmark: nop.so benchmark.c
	cc -D BUFLEN=1000000000 -O0 -s -o benchmark benchmark.c

nop.so: nop.c
	cc -O0 -s -shared -o nop.so nop.c

clean:
	rm -f nop.so benchmark

.PHONY: clean