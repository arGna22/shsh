#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "argslinkedl.h"
// Don't forget relevant import statements
// Port over everythign, create a remove function as well
// Make sure to deal with memory leaks later.


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

// Removes the most recent element with the corresponding cmd value.
// Returns 0 on success and -1 on failure 
// We will worry about the whole "the args starts with an operator" thing after this works
// I do not think I will need this anymore, but it will be kept around just in case
int removeNode(struct arg *args, char *cmd)
{
    int status = -1; 
    struct arg *previous = args;
    for (struct arg *i = args; i != NULL; i = i->next) {
        if (!strcmp(i->cmd, cmd)) {
            previous->next = i->next->next;    
            return 0;
        }
        previous = i;
    }
}