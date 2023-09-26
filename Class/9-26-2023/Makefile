9-7: 9-7.o stack.o
	gcc 9-7.o stack.o -o 9-7

9-7.o: 9-7.c stack.h
	gcc -Wall -Werror -ggdb 9-7.c -c

stack.o: stack.c stack.h
	gcc -Wall -Werror -ggdb stack.c -c

clean:
	rm -f *.o 9-7