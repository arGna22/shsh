main: main.c utils.o builtins.o signalhandling.o argslinkedl.o
	gcc main.c utils.o builtins.o signalhandling.o argslinkedl.o -o main

utils.o: utils.c builtins.c  
	gcc -c utils.c -o utils.o 

builtins.o: builtins.c
	gcc -c builtins.c -o builtins.o

signalhandling.o: signalhandling.c 
	gcc -c signalhandling.c -o signalhandling.o

argslinkedl.o: argslinkedl.c
	gcc -c argslinkedl.c -o argslinkedl.o

testing: testing.c utils.o builtins.o argslinkedl.o
	gcc testing.c utils.o builtins.o argslinkedl.o -o testing

maindebug: main.c utils.o builtins.o signalhandling.o
	gcc -fsanitize=address, undefined main.c utils.o builtins.o signalhandling.o -o main 

