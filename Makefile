CC=clang

all: test4x4 test8x8

test4x4: wht4x4.c
	$(CC) -O3 -march=native -DWHT_TEST -o $@ $<
test8x8: wht8x8.c
	$(CC) -O3 -march=native -DWHT_TEST -o $@ $<

clean:
	rm -f test* *.i *.o *.s *.bc