Path-Mapping: Path-Mapping.o heap.o
	gcc -ggdb Path-Mapping.o heap.o -o Path-Mapping -lm

Path-Mapping.o: Path-Mapping.c heap.h
	gcc -Wall -Werror -ggdb Path-Mapping.c -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -ggdb heap.c -c

clean:
	rm -f *.o Path-Mapping