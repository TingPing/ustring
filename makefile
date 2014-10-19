CFLAGS=-std=c99 \
		-Wall -Wextra -Werror -pedantic \
		-Wunused-macros \
		--coverage
CC?=gcc

test: src/*.c
	$(CC) $(CFLAGS) $^ -o $@

run: test
	./test

coverage:
	coveralls --gcov-options '\-lp'

clean:
	rm -f ./test*
	rm -f ./ustring.*

.PHONY: clean run coverage
