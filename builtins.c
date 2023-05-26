#include <unistd.h>
#include "utils.h"

void cd(struct arg *args) // This needs to accept a linked list of arguments
{
	if (args->next)
		chdir(args->next->cmd);	
	else
		chdir("~");
}
