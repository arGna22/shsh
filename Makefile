main: main.c utils.o builtins.o
	gcc main.c utils.o builtins.o -o main

utils.o: utils.c builtins.c  
	gcc -c utils.c -o utils.o 

builtins.o: builtins.c
	gcc -c builtins.c -o builtins.o

testing: testing.c utils.o builtins.o 
	gcc testing.c utils.o builtins.o -o testing

maindebug:
	gcc -g main.c utils.o builtins.o -o main 

