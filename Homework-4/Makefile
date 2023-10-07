Path-Finding: Path-Finding.o heap.o
	gcc -ggdb Path-Finding.o heap.o -o Path-Finding -lm -lc

Path-Finding.o: Path-Finding.c heap.h
	gcc -Wall -Werror -ggdb Path-Finding.c -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -ggdb heap.c -c

clean:
	rm -f *.o Path-Finding