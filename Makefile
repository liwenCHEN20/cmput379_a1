

all: dir build/get_mem_layout_test

dir: 
	mkdir build

build/get_mem_layout_test: build/test.o build/memchunk.o
	gcc build/test.o build/memchunk.o -m32 -std=c99 -o build/get_mem_layout_test

build/test.o: test.c
	gcc -c -m32 -std=c99 test.c -o build/test.o

build/memchunk.o: memchunk.c
	gcc -c -m32 -std=c99 memchunk.c -o build/memchunk.o

clean:
	rm -rf build

