#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "argslinkedl.h"

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
	if (i == ARGSLIMIT) {
		i--;
		return;
	}
	arr[i] = NULL; 
}

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