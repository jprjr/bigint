.PHONY: all clean test

CFLAGS = -fPIC -g -O0 -Wall -Wextra
LDFLAGS =

all: utest-byte utest-short utest-word utest-quad \
     utest-byte-static utest-short-static utest-word-static utest-quad-static \
     utest-byte-single utest-short-single utest-word-single utest-quad-single \
	 utest-coverage bigint.o

test: utest-byte utest-short utest-word utest-quad utest-byte-single utest-short-single utest-word-single utest-quad-single utest-byte-static utest-short-static utest-word-static utest-quad-static utest-coverage
	./utest-byte >/dev/null 2>&1 || ./utest-byte
	./utest-short >/dev/null 2>&1 || ./utest-short
	./utest-word >/dev/null 2>&1 || ./utest-word
	./utest-quad >/dev/null 2>&1 || ./utest-quad
	./utest-byte-single >/dev/null 2>&1 || ./utest-byte-single
	./utest-short-single >/dev/null 2>&1 || ./utest-short-single
	./utest-word-single >/dev/null 2>&1 || ./utest-word-single
	./utest-quad-single >/dev/null 2>&1 || ./utest-quad-single
	./utest-byte-static >/dev/null 2>&1 || ./utest-byte-static
	./utest-short-static >/dev/null 2>&1 || ./utest-short-static
	./utest-word-static >/dev/null 2>&1 || ./utest-word-static
	./utest-quad-static >/dev/null 2>&1 || ./utest-quad-static
	mkdir -p coverage
	rm -f coverage/*
	./utest-coverage
	gcovr --html-details --output coverage/index.html
	rm -f *.gcda
	rm -f *.gcno

utest-byte: utest-byte.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-short: utest-short.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-word: utest-word.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-quad: utest-quad.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-byte-single: utest-byte-single.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-short-single: utest-short-single.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-word-single: utest-word-single.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-quad-single: utest-quad-single.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-byte-static: utest-byte-static.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-short-static: utest-short-static.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-word-static: utest-word-static.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-quad-static: utest-quad-static.o
	$(CC) -o $@ $^ $(LDFLAGS)

utest-coverage: utest-coverage.o
	$(CC) -o $@ $^ $(LDFLAGS) --coverage

utest-byte.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_WORD_WIDTH=1 -c -o $@ $<

utest-short.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_WORD_WIDTH=2 -c -o $@ $<

utest-word.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_WORD_WIDTH=4 -c -o $@ $<

utest-quad.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_WORD_WIDTH=8 -c -o $@ $<

utest-byte-single.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_SINGLE_WORD_ONLY -DBIGINT_WORD_WIDTH=1 -c -o $@ $<

utest-short-single.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_SINGLE_WORD_ONLY -DBIGINT_WORD_WIDTH=2 -c -o $@ $<

utest-word-single.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_SINGLE_WORD_ONLY -DBIGINT_WORD_WIDTH=4 -c -o $@ $<

utest-quad-single.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_SINGLE_WORD_ONLY -DBIGINT_WORD_WIDTH=8 -c -o $@ $<

utest-byte-static.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_NO_MALLOC -DBIGINT_WORD_WIDTH=1 -c -o $@ $<

utest-short-static.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_NO_MALLOC -DBIGINT_WORD_WIDTH=2 -c -o $@ $<

utest-word-static.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_NO_MALLOC -DBIGINT_WORD_WIDTH=4 -c -o $@ $<

utest-quad-static.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) -DBIGINT_NO_MALLOC -DBIGINT_WORD_WIDTH=8 -c -o $@ $<

utest-coverage.o: utest.c utest.h bigint.h
	$(CC) $(CFLAGS) --coverage -c -o $@ $<

bigint.o: bigint.h
	$(CC) -DBIGINT_IMPLEMENTATION -std=c99 -pedantic $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.exe
	rm -f utest-coverage
	rm -f utest-byte utest-short utest-word utest-quad
	rm -f utest-byte-single utest-short-single utest-word-single utest-quad-single
	rm -f utest-byte-static utest-short-static utest-word-static utest-quad-static
