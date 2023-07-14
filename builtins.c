#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <pwd.h>
#include "utils.h"
#include "argslinkedl.h"

void cd(struct arg *args) 
{
	struct passwd *pw = getpwuid(getuid());
	const char *homedir = pw->pw_dir;
	if (args->next)
		chdir(args->next->cmd);	
	else
		chdir(homedir);
}

void killProgram(struct arg *args)
{
	exit(0);
}
