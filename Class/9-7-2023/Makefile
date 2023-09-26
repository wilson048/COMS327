queue-test: queue-test.o queue.o
	gcc queue-test.o queue.o -o queue-test

queue-test.o: queue-test.c queue.h
	gcc -Wall -Werror -ggdb queue-test.c -c

queue.o: queue.c queue.h
	gcc -Wall -Werror -ggdb queue.c -c

clean:
	rm -f *.o queue-test

#This would work, but it's not the "right" way
#queue: 9-7.c queue.c queue.h 
#	gcc -Wall -Werror 9-7.c queue.c -o queue