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
	puts(args[0]);
	for (int i = 0; i < NUM_BUILTINS; i++) {
		if (!strcmp(builtinsNames[i], args[0])) {
			puts("Found match!");
			(builtins[i])(argc, args);
			return 0;
		}
	}
	puts("Did not find match!");

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
	char currentdir[150];
	getcwd(currentdir, 150);

	char *username = getenv("USER");
	char hostname[15];

	
	char command[4096];
	char *args[15]; 
	int argc;

	gethost(hostname);

	for (;;) {
		printf("[%s@%s]%s$ ", username, hostname, currentdir);
		fgets(command, 4096, stdin);
		command[strlen(command) - 1] = '\0';
		puts(command);
		argc = strsplit(command, ' ', args, 15);

		if (searchBuiltins(argc, args) == -1)
			runCmd(args);
		getcwd(currentdir, 150);
	} 

	return 0;
}
