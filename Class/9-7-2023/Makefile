9-7: 9-7.o queue.o
	gcc 9-7.o queue.o -o 9-7

9-7.o: 9-7.c queue.h
	gcc -Wall -Werror -ggdb 9-7.c -c

queue.o: queue.c queue.h
	gcc -Wall -Werror -ggdb queue.c -c

clean:
	rm -f *.o 9-7

#This would work, but it's not the "right" way
#queue: 9-7.c queue.c queue.h 
#	gcc -Wall -Werror 9-7.c queue.c -o queue