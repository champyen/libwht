CC=clang

test: wht4x4.c
	$(CC) -march=native -DWHT_TEST -o $@ $<

clean:
	rm -f test *.i *.o *.s