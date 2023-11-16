use-heap: use-heap.o heap.o libheap.a
	gcc use-heap.o -o use-heap -l heap -L.
heap.o: heap.c heap.h
	gcc -Wall -Werror -ggdb heap.c heap.h -c
libheap.a: heap.o
	ar -cvq libheap.a heap.o
use-heap.o: use-heap.c
	gcc -Wall -Werror -ggdb use-heap.c -c -I.
libprintf.so: printf.c
	# gcc -shared -o libprintf.so -fPIC printf.c
	gcc -Wall -Werror -ggdb printf.c -c
	gcc -shared -Wl, -soname, libprintf.so -o libprintf.so printf.o
test: test.c
	gcc -Wall -Werror -ggdb test.c -o test
clean:
	rm -f *.o use-heap libheap.a *.h.gch *.so