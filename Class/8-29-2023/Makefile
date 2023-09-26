all: random-ints HelloWorld.class

random-ints: random-ints.c
	gcc -Wall -Werror random-ints.c -o random-ints
HelloWorld.class: HelloWorld.java
	javac HelloWorld.java

clean:
	rm -f random-ints HelloWorld.class *.o