CC := gcc
CFLAGS := -O3 -Wall
CFLAGS += -ggdb3
LIB := build.o lib.o /usr/local/lib/liblac.a

all:
	$(CC) $(CFLAGS) -c -o lib.o src/lib.c
	$(CC) $(CFLAGS) -c -o build.o src/build.c
	$(CC) $(CFLAGS) -o bin/prop2code src/main.c $(LIB)

install:
	strip bin/prop2code
	gzexe bin/prop2code
	cp bin/prop2code /usr/local/bin

deinstall:
	rm -f /usr/local/bin/prop2code

clean:
	rm -rf lib.o bin/*
