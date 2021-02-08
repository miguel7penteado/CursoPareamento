CC = gcc
CFLAGS = -O0 -g -Wall -Wextra -pedantic `pkg-config --cflags libxml-2.0` -I..
LDFLAGS = `pkg-config --libs libxml-2.0` -lm -pthread -lz -lcmocka

.PHONY: clean
.SILENT: clean

all: record_test

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

../%.o:	../%c
	cd ..
	make $@

run: all
	./record_test

val: all
	valgrind --leak-check=full --show-leak-kinds=all ./record_test

record_test: ../record.o record_test.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f *.o record_test
