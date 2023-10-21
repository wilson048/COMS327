Player-Movement: Player-Movement.o heap.o
	gcc -ggdb Player-Movement.o heap.o -o Player-Movement -lm -lc

Player-Movement.o: Player-Movement.c heap.h
	gcc -Wall -Werror -ggdb Player-Movement.c -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -ggdb heap.c -c

clean:
	rm -f *.o Player-Movement