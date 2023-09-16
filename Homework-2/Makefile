Terrain-Movement: Terrain-Movement.o Local_Map.o
	gcc -ggdb -Wall -Werror Terrain-Movement.o Local_Map.o -o Terrain-Movement -fno-stack-protector -lm
Terrain-Movement.o:
	gcc -Wall -Werror -ggdb Terrain-Movement.c -fno-stack-protector -lm -c
Local_Map.o:
	gcc -Wall -Werror -ggdb Local_Map.c -fno-stack-protector -lm -c
clean:
	rm -f *.o Terrain-Movement 