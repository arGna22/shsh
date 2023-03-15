#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void error(char *msg); 
void gethost(char *var);

void error(char *msg) 
{
	fprintf(stderr, "%s: %s", msg, strerror(errno));
	exit(1);
}

void gethost(char *var) 
{
	pid_t pid;
	FILE *fp;
	int fd[2];

	if (pipe(fd) == -1)
		error("Can't create pipe");
	
	if ((pid = fork()) == -1) {
		error("Unable to fetch hostname");
	} else if (!pid) {
		close(fd[0]);
		dup2(fd[1], 1);
		if (execl("/bin/hostname", "/bin/hostname", NULL) == -1) 
			error("Could not execute 'hostname'");
	}

	close(fd[1]);
	fp = fdopen(fd[0], "r"); 
	fscanf(fp, "%s", var);
}

int main() 
{
	char *currentdir;
	char *username = getenv("USER");
	char command[4096];
	char hostname[15];

	gethost(hostname);

	for (;;) {
		currentdir = getenv("PWD");
		printf("[%s@%s]%s$ ", username, hostname, currentdir);
		fgets(command, 4096, stdin);
		command[strlen(command) - 1] = '\0';
	} 

	return 0;
}
