#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "utils.h"
#include "builtins.h"


#define NUM_BUILTINS 1 


void (*builtins[])(int argc, char **args)= {cd};
char *builtinsNames[NUM_BUILTINS] = {
	"cd"
};

int searchBuiltins(int argc, char **args, void (*builtins[])(int argc, char **args));

int searchBuiltins(int argc, char **args, void (*builtins[])(int argc, char **args)) 
{
	for (int i = 0; i < NUM_BUILTINS; i++) {
		if (!strcmp(builtinsNames[i], args[0])) {
			(builtins[i])(argc, args);
			return 0;
		}
	}

	return -1;
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
		argc = strsplit(command, ' ', args, 15);

		if (searchBuiltins(argc, args, builtins) == -1) { 
			args[argc] = NULL;
			runCmd(args);
		}
		getcwd(currentdir, 150);
	} 

	return 0;
}
