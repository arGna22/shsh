#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "utils.h"

int main() 
{
	char *currentdir;
	char *username = getenv("USER");
	char command[4096];
	char hostname[15];

	gethost(hostname);

	for (;;) {
		currentdir = getenv("PWD");
		printf("[%s@%s]%s$ ", username, hostname, currentdir);
		fgets(command, 4096, stdin);
		command[strlen(command) - 1] = '\0';
	} 

	return 0;
}
