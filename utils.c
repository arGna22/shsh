#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "utils.h"
#include "builtins.h"
#include "argslinkedl.h"

#define NUMBUILTINS 2

char *builtinList[] = {"cd", "exit"};
void (*builtins[])(struct arg *args) = {cd, killProgram};

int placeholderDescriptor = 3;

void error(char *msg, int end)
{
	fprintf(stderr, "\n%s: %s", msg, strerror(errno));
	if (end) exit(1); // This should work, I believe.
}

char *trimWhitespace(char *str)
{
	// Loop until you find a letter or null character
	while (*str == ' ') {str++;}
	return str;
}

struct arg *strsplit(char *input)  
{
	char *token = strtok(input, " ");
	struct arg *argumentList = NULL;
	struct arg *current = NULL;
	while (token != NULL) {
		if (!argumentList) {
			argumentList = create(token);
			current = argumentList;
		} else {
			current->next = create(token);
			current = current->next;
		}
		token = strtok(NULL, " ");
	}

	return argumentList;
}

void exeCmd(struct arg *args)
{
	/// Redirection should happne here.
	// If the user chooses to redirect stdewrr, then that's there problem.
	char *commandName = args->cmd;

	for (int i = 0; i < NUMBUILTINS; i++) {
		if (!strcmp(builtinList[i], commandName)) {
			(builtins[i])(args);
			return;
		}
	}

	char *arguments[ARGSLIMIT];
	redirectOutput(args);
	getArgsArray(args, arguments);

	pid_t pid;
	int pidstatus;
	pid = fork();

	if (pid == -1)
		error("Unable to fork process", 0);
	if (!pid)
	{
		struct arg *current = args;
		if(execvp(args->cmd, arguments) == -1)
			error("Command unable to be executed", 0); // should i exit, or should i not?
	}


	if (waitpid(pid, &pidstatus, 0) == -1 && pid != -1)
		error("Error waiting for process", 0);
	
	// The only reason I am skeptical to use exec is because if a user provides a path toa commnd, it will be searched for in path, which will give an incorrect result. 
	// For now, I will use execvp for the sake of simplicity. 
	// Fork and execute comand using execvp
	// OR exec
	// if it is in the list of builtins, then run the builtin.
	// if it is not in the list of builtins, then execute it.
	
}

int redirectOutput(struct arg *args)
{
	char *text = NULL;
	int outc = 0;
	int inc = 0;
	int errc = 0;
	char *filename = NULL;
	FILE *target = NULL; 
	for (struct arg *i = args; i != NULL; i = i->next)
	{
		text = i->cmd;
		if (!strcmp(text, ">")) {
			outc++;
			if (outc > 1)
			{
				fprintf(stderr, "Error: You cannot utilize more than one stdout redirection operator per command.\n");
				return -1;
			}

			if (i->next != NULL)  
				filename = i->next->cmd;
			else
			{
				fprintf(stderr, "Error: Redirection operator with no target.\n");
				return -1;
			}
			dup2(STDOUT_FILENO, 3);

			target = fopen(filename, "w");
			dup2(fileno(target), STDOUT_FILENO);
			close(fileno(target));
		} 
		else if (!strcmp(text, ">>")) {
			outc++;
			if (outc > 1)
			{
				fprintf(stderr, "Error: You cannot utilize more than one stdout redirection operator per command.\n");
				return -1;
			}
			if (i->next != NULL)
				filename = i->next->cmd;
			else
			{
				fprintf(stderr, "Error: Redirection operator with no target.\n");
				return -1;
			}
			dup2(STDOUT_FILENO, 3);

			target = fopen(filename, "a");
			dup2(fileno(target), STDOUT_FILENO);
			close(fileno(target));

		} // stdout append
		else if (!strcmp(text, ">>>")) {
			errc++;
			if (errc > 1)
			{
				fprintf(stderr, "Error: You cannot utilize more than one stderr redirection operator per command.\n");
				return -1;
			}
			if (i->next != NULL)
				filename = i->next->cmd;
			else
			{
				fprintf(stderr, "Error: Redirection operator with no target.\n");
				return -1;
			}
			dup2(STDERR_FILENO, 4);

			target = fopen(filename, "w");
			dup2(fileno(target), STDERR_FILENO);
			close(fileno(target));

		} // stderr 
		else if (!strcmp(text, "<")) {
			inc++;
			if (inc > 1)
			{
				fprintf(stderr, "Error: You cannot utilize more than one stdin redirection operator per command.\n");
				return -1;
			}
			if (i->next != NULL)
				filename = i->next->cmd;
			else
			{
				fprintf(stderr, "Error: Redirection operator with no target.\n");
				return -1;
			}
			dup2(STDIN_FILENO, 5);

			target = fopen(filename, "r");
			if (target == NULL) {
				error("Error: file could not be opened.", 0);
				return -1;
			}
			dup2(fileno(target), STDIN_FILENO);
			close(fileno(target));
		} 
	}
	return 0;
}

// called after redirect
void cleanArgs(struct arg *args)
{
	struct arg *previous = NULL;
	char *operator = NULL;
	for (struct arg *i = args; i != NULL; i = i->next)
	{
		operator = i->cmd;
		if (strchr(operator, '>') || strchr(operator, '<'))
		{
			previous->next = i->next->next;
			// We need to free stuff too... Otheriwse, we get memory leaks...
			i = previous;
		}
		else
		{
			previous = i;
		}
	}
}

void resetDescriptorTable()
{
	// if this doesn't work, then I can try to do something with the counts and if statements.
	dup2(3, STDOUT_FILENO);
	dup2(4, STDERR_FILENO);
	dup2(5, STDIN_FILENO);
}

