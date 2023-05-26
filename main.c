#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "prompt.h"
#include "utils.h"
#include "builtins.h"

#define HOSTNAMESIZE 64
#define PATHNAMESIZE 4096
#define INPUTSIZE 500 

int main()
{
	/* VARIABLE DECLARATIONS */

	// Prompt-related variables
	char hostname[HOSTNAMESIZE];
	char *username;
	char cwd[PATHNAMESIZE];

	// Activity-related variables 
	char input[INPUTSIZE];
	char *command;
	struct arg *arguments;

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
	for (;;) {
		getcwd(cwd, PATHNAMESIZE);
		// Display the prompt
		printf("%s@%s:[%s]$ ", username, hostname, cwd);
		// Wait for/collect command 
		fgets(input, INPUTSIZE, stdin);
		// Remove the newline
		input[strcspn(input, "\n")] = '\0';
		// Checking if the string read in is empty
		if (input[0] == '\0')
			continue;
		arguments = strsplit(input);
		command = arguments->cmd; 

		// Execute the command appropriately
		exeCmd(arguments); 
	}
		
	return 0;
}
