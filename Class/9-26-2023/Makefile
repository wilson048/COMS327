stack-test: stack-test.o stack.o
	gcc stack-test.o stack.o -o stack-test

stack-test.o: stack-test.c stack.h
	gcc -Wall -Werror -ggdb stack-test.c -c

stack.o: stack.c stack.h
	gcc -Wall -Werror -ggdb stack.c -c

clean:
	rm -f *.o stack-test