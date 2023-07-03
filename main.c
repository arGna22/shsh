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
	/* SIGNAL HANDLING */
	if (catch_signal(SIGINT, interrupt) == -1) {
		fprintf(stderr, "Can't map the handler");
	}

	/* VARIABLE DECLARATIONS */

	// Prompt-related variables
	char hostname[HOSTNAMESIZE];
	char *username;
	char cwd[PATHNAMESIZE];

	// Activity-related variables 
	char input[INPUTSIZE];
	char *trimmedInput;
	char *command;
	struct arg *arguments;


	// fgets return value
	char *returnVal;

	// Fetching and setting information for prompt
	gethostname(hostname, HOSTNAMESIZE);
	username = getlogin();
	if (!username) {
		fprintf(stderr, "Error: unable to fetch username");
		return 1;
	}
	getcwd(cwd, PATHNAMESIZE);
	struct promptInfo prompt = {hostname, username, cwd};

	// Running the shell program
	do {
		getcwd(cwd, PATHNAMESIZE);
		// Display the prompt
		printf("%s@%s:[%s]$ ", username, hostname, cwd);

		// Wait for/collect command 
		returnVal = fgets(input, INPUTSIZE, stdin);
		if (!input)
			return 0;
		
		// This prevents CTRL+C from being accidentally interpreted as a command.
		if (!strchr(input, '\n')) {
			printf("\n");
			continue;
		}
		// Remove the newline
		input[strcspn(input, "\n")] = '\0';
		
		// Trim the whitespace of the input
		trimmedInput = trimWhitespace(input);

		// Checking if the string read in is empty
		if (trimmedInput[0] == '\0')
			continue;

		arguments = strsplit(input);

		// Massive memory leak between commands I need to fix this by releasing the entire L.L.
		command = arguments->cmd; 

		if (redirectOutput(arguments) != -1) {
			cleanArgs(arguments);
			// we need another trimmed input check here...
			// Execute the command appropriately
			exeCmd(arguments);  // The I/O redirection will take place in this command. 

		}
		resetDescriptorTable();
	} while(returnVal);
		
	return 0;
}
