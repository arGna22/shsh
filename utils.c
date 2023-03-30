#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include "utils.h"

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

int strsplit(char *str, char delim, char **strings, int len)
{

        int state = 1; 
        int count = 0;
        int substrCount = 0;
        char substr[500]; 

        for (char *s = str; 1; s++) {
                if (*s == delim && state || substrCount > 13 || !(*s)) {
                        substr[substrCount] = '\0';
			strings[count] = strdup(substr);
                        count++;
			if (!(*s))
				return count; 
                        state = 0;
                        substrCount = 0;
                }
                else if (*s != delim) {
                        state = 1;
                        substr[substrCount] = *s;
                        substrCount++;
                }
        }
}

void ioredir(struct redirInfo *info)
{
	FILE *redirect;
	char *mode;
	
	for (int i = 0; i < info->len; i++) {
		
		switch (info->streams[i]) {
		case IN:
			mode = "r";
			break;
		case OUT:
			mode = "w";
			break;
		case ERR:
			mode = "a";
		}
		info->redirect = fopen(info->filename, mode);
		if (dup2(info->streams[i], fileno(info->redirect)) == -1)
			perror("Unable to copy stream");
	}

}

void runCmd(char **args, int redir, struct redirInfo *info) // redir is 1 if there was a redirection, 0 if not
{
	pid_t pid; 
	int pidstatus;
	pid = fork();

	if (pid == -1)
		perror("Unable to fork process");
	if (!pid) {
		if (redir) 
			ioredir(info);
		
		if (execvp(args[0], args) == -1) {
			perror("Error executing command");
		}
	}

	if (waitpid(pid, &pidstatus, 0) == -1 && pid != -1)
		perror("Error waiting for process.");
}

