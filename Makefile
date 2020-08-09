CC=clang

test: wht4x4.c
	$(CC) -O3 -march=native -DWHT_TEST -o $@ $< --save-temps

clean:
	rm -f test *.i *.o *.s *.bc