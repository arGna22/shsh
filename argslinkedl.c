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

void freeList(struct arg **args) {
	if (*args != NULL) {
		if ((*args)->next == NULL) {
			free(*args); 
			*args = NULL;
		} else {
			struct arg *i = *args;
			struct arg *current = NULL;
			while (i != NULL) {
				current = i;
				i = i->next;
				free(current->cmd);
				free(current);
			}
		}
	}
}

