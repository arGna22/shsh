utils.o: utils.c
	gcc -c utils.c -o utils.o

main.o: main.c
	gcc -c main.c -o main.o

main: main.o utils.o
	gcc utils.o main.o -o main