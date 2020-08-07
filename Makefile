CC=clang

test: wht4x4.c
	$(CC) -O3 -march=native -DWHT_TEST -o $@ $< 

clean:
	rm -f test