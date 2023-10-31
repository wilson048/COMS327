c-to-cpp: c-port.o c-to-cpp.o
	g++ c-to-cpp.o c-port.o -o c-to-cpp

c-port.o: c-port.c c-port.h
	gcc -Wall -Werror -ggdb c-port.c -c

c-to-cpp.o: c-to-cpp.cpp
	g++ -Wall -Werror -ggdb c-to-cpp.cpp -c

clean:
	rm -f *.o c-to-cpp