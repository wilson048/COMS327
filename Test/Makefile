poke327: poke327.o heap.o
	g++ -ggdb poke327.o heap.o -o poke327 -lm -lc -lncurses

poke327.o: poke327.c heap.h
	g++ -Wall -Werror -ggdb poke327.cpp -c

heap.o: heap.c heap.h
	gcc -Wall -Werror -ggdb heap.c -c

clean:
	rm -f *.o poke327 fuckup