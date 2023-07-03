#include <unistd.h>
#include <signal.h>
#include "utils.h"

void cd(struct arg *args) // This needs to accept a linked list of arguments
{
	if (args->next)
		chdir(args->next->cmd);	
	else
		chdir("~");
}

// THe text for this builtin will be "exit"
void killProgram(struct arg *args)
{
	raise(SIGKILL); // I might make this a different signal so that I can actually handle it and do soem necessary operatiosn before the program dies.
}
