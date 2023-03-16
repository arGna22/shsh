#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <stdarg.h>
#include "utils.h"
#include "builtins.h"


#define NUM_BUILTINS 1 


void (*builtins[])(int argc, char **args)= {cd};
char *builtinsNames[NUM_BUILTINS] = {
	"cd"
};

int searchBuiltins(int argc, char **args);

int searchBuiltins(int argc, char **args) 
{
	for (int i = 0; i < NUM_BUILTINS; i++) {
		if (!strcmp(builtinsNames[i], args[0])) {
			builtins[i](argc, args);
			return 0;
		}
	}

	return -1;
}

void runCmd(char **args) 
{
	pid_t pid;
	pid = fork();
	if (pid == -1) {
		if (execvp(args[0], args) == -1)
			fprintf(stderr, "shsh: command: '%s' not found", args[0]); 
	}
}
int main() 
{
	char *currentdir;
	char *username = getenv("USER");
	char hostname[15];

	
	char command[4096];
	char *args[15]; 
	int argc;

	gethost(hostname);

	for (;;) {
		currentdir = getenv("PWD");
		printf("[%s@%s]%s$ ", username, hostname, currentdir);
		fgets(command, 4096, stdin);
		command[strlen(command) - 1] = '\0';
		argc = strsplit(command, ' ', args, 15);

		if (searchBuiltins(argc, args) == -1)
			runCmd(args);
	} 

	return 0;
}
