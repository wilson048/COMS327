9-7: 9-7.o queue.o
	gcc 9-7.o queue.o -o 9-7

9-7.o: 9-7.c queue.h
	gcc -Wall -Werror -ggdb 9-7.c -c

queue.o: queue.c queue.h
	gcc -Wall -Werror -ggdb queue.c -c

clean:
	rm -f *.o 9-7