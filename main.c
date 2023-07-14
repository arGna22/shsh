#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include "prompt.h"
#include "utils.h"
#include "builtins.h"
#include "signalhandling.h"
#include "argslinkedl.h"

#define HOSTNAMESIZE 64
#define PATHNAMESIZE 4096
#define INPUTSIZE 500 

int main()
{
	signal(SIGINT, SIG_IGN);

	char hostname[HOSTNAMESIZE];
	char *username; 
	char cwd[PATHNAMESIZE];

	char input[INPUTSIZE];
	char *trimmedInput;
	char *command;
	struct arg *arguments;

	char *returnVal;

	gethostname(hostname, HOSTNAMESIZE);
	username = getlogin();
	if (!username) {
		fprintf(stderr, "Error: unable to fetch username");
		return 1;
	}
	getcwd(cwd, PATHNAMESIZE);
	struct promptInfo prompt = {hostname, username, cwd};

	do {
		getcwd(cwd, PATHNAMESIZE);
		printf("%s@%s:[%s]$ ", username, hostname, cwd);

		returnVal = fgets(input, INPUTSIZE, stdin);
		if (!input)
			return 0;
		
		if (!strchr(input, '\n')) {
			printf("\n");
			continue;
		}
		input[strcspn(input, "\n")] = '\0';
		
		trimmedInput = trimWhitespace(input);

		if (trimmedInput[0] == '\0')
			continue;

		arguments = strsplit(input);

		command = arguments->cmd; 

		if (redirectOutput(arguments) != -1) {
			cleanArgs(&arguments);
			if (arguments != NULL)
				exeCmd(arguments); 
		}
		resetDescriptorTable();
		freeList(&arguments);
	} while(returnVal);
		
	return 0;
}
