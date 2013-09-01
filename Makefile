CC := gcc
CXX := g++
CFLAGS := -O3 -Wall
CFLAGS += -ggdb3
CXXFLAGS := -O3 -Wall
CXXFLAGS += -ggdb3
LIB := build_c.o build_cpp.o build.o lib.o /usr/local/lib/libla.a

all:
	mkdir -p bin
	$(CC) $(CFLAGS) -c -o lib.o src/lib.c
	$(CC) $(CFLAGS) -c -o build.o src/build.c
	$(CC) $(CFLAGS) -c -o build_c.o src/build_c.c
	$(CC) $(CFLAGS) -c -o build_cpp.o src/build_cpp.c
	$(CC) $(CFLAGS) -o bin/prop2code src/main.c $(LIB)

template:
	$(CC) $(CFLAGS) -o tpl/type_c tpl/type.c /usr/local/lib/libla.a
	$(CXX) $(CXXFLAGS) -o tpl/type_cpp tpl/type.cpp /usr/local/lib/libla++.a

test:
	bin/prop2code -f test.properties -t c -n test
	echo "#include <stdio.h>" > test_c.c
	echo "#include \"test.h\"" >> test_c.c
	echo "int main(void) {" >> test_c.c
	echo "	test_load(\"test.properties\");" >> test_c.c
	echo "" >> test_c.c
	echo "	test_show();" >> test_c.c
	echo "" >> test_c.c
	echo "	test_save(\"test_c.backup\");" >> test_c.c
	echo "" >> test_c.c
	echo "	return 0;" >> test_c.c
	echo "}" >> test_c.c
	$(CC) $(CFLAGS) -o test_c test_c.c test.c /usr/local/lib/libla.a
	./test_c
	bin/prop2code -f test.properties -t cpp -n test

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
