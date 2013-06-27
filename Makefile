CC := gcc
CFLAGS := -O3 -Wall
CFLAGS += -ggdb3
LIB := build_c.o build.o lib.o /usr/local/lib/libla.a

all:
	$(CC) $(CFLAGS) -c -o lib.o src/lib.c
	$(CC) $(CFLAGS) -c -o build.o src/build.c
	$(CC) $(CFLAGS) -c -o build_c.o src/build_c.c
	$(CC) $(CFLAGS) -o bin/prop2code src/main.c $(LIB)

template:
	$(CC) $(CFLAGS) -o tpl/type_c tpl/type_c.c $(LIB)

test:
	bin/prop2code -f test.properties -n test_c
	echo "#include <stdio.h>" > test_c_main.c
	echo "#include \"test_c.h\"" >> test_c_main.c
	echo "int main(void) {" >> test_c_main.c
	echo "	test_c_load(\"test.properties\");" >> test_c_main.c
	echo "" >> test_c_main.c
	echo "	test_c_show();" >> test_c_main.c
	echo "" >> test_c_main.c
	echo "	test_c_save(\"test_c.backup\");" >> test_c_main.c
	echo "" >> test_c_main.c
	echo "	return 0;" >> test_c_main.c
	echo "}" >> test_c_main.c
	$(CC) $(CFLAGS) -c -o test_c.o test_c.c
	$(CC) $(CFLAGS) -o test_c test_c_main.c test_c.o /usr/local/lib/libla.a
	./test_c

install:
	strip bin/prop2code
	gzexe bin/prop2code
	cp bin/prop2code /usr/local/bin

deinstall:
	rm -f /usr/local/bin/prop2code

clean:
	rm -f *.o
	rm -f bin/*
	rm -f test_c*
