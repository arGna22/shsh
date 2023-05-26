#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include "utils.h"
#include "builtins.h"

#define NUMBUILTINS 1
#define ARGSLIMIT 20
#define ARGSLENLIMIT 4096

char *builtinList[] = {"cd"};
void (*builtins[])(struct arg *args) = {cd};

void error(char *msg, int end)
{
	fprintf(stderr, "\n%s: %s", msg, strerror(errno));
	if (end) exit(1); // This should work, I believe.
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

struct arg *create(char *text)
{
	struct arg *argument = malloc(sizeof(struct arg));
	argument->cmd = strdup(text);
	argument->next = NULL;

	return argument; 
}

void getArgsArray(struct arg *args, char *arr[])
{
	struct arg *current = args;
	int i = 0;

	for (; i < ARGSLIMIT - 1 && current != NULL; i++)
	{
		arr[i] = current->cmd;
		current = current->next;
	}
	// If i == ARGSLIMIT then index will be out of bounds, so we must decrement
	if (i == ARGSLIMIT) {
		i--;
		return;
	}
	arr[i] = NULL; 
}

void exeCmd(struct arg *args)
{
	char *commandName = args->cmd;
	for (int i = 0; i < NUMBUILTINS; i++) {
		if (!strcmp(builtinList[i], commandName)) {
			(builtins[i])(args);
			return;
		}
	}

	char *arguments[ARGSLIMIT];
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

