#include <unistd.h>

void cd(int argc, char **args) 
{
	chdir(args[1]);
}
