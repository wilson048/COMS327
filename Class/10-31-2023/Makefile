cpp-to-c: cpp-port.o cpp-to-c.o
	g++ cpp-to-c.o cpp-port.o -o cpp-to-c

cpp-port.o: cpp-port.cpp cpp-port.h
	g++ -Wall -Werror -ggdb cpp-port.cpp -c

cpp-to-c.o: cpp-to-c.c
	gcc -Wall -Werror -ggdb cpp-to-c.c -c

clean:
	rm -f *.o cpp-to-c