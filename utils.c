#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>


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

/*int strsplit(char *str, char delim, char **strings, int len) 
{
	int elements = 0; 
	int index = 0;
	int count = 0; 
	char substr[60]; 
	char *memlocs[50];
	char *beginning = str;
	char *end;
	char *j = str;
	int i = 0;

	while (*j && i < 50) {
		if (*j == delim) {
			memlocs[i] = j; 
			i++;
		}
		j++;
	}

	memlocs[i] = strchr(str, '\0'); 
	for (int k = 0; k <= i; k++) {
		end = memlocs[k];
		for (char *m = beginning; m < end; m++) { 
			if (*m != delim) {
				substr[index] = *m;
				index++;
			}
		}

		substr[index] = '\0';
		index = 0; 
		if (!strchr(substr, ' '))
			strings[count] = strdup(substr);
		count++; 
		beginning = end;
	}


	return count;
}*/

int strsplit(char *str, char delim, char strings[15][30], size_t len)
{

        int state = 1; // 1 for in, 0 for out.
        int count = 0;
        int substrCount = 0;
        char substr[15]; // I could actually make this the size of element len

        for (char *s = str; 1; s++) { // This is good. It'll copy the null character too.
                if (*s == delim && state || substrCount > 13 || !(*s)) {
                        substr[substrCount] = '\0';
                        strncpy(strings[count], substr, 15);
			if (!(*s))
				return count; // Used to be break;
                        count++;
                        state = 0;
                        substrCount = 0;
                }
                else if (*str != delim) {
                        state = 1;
                        substr[substrCount] = *s;
                        substrCount++;
                }
        }
}

void runCmd(char **args)
{
	pid_t pid;
	int pidstatus;
	pid = fork();
	if (pid == -1)
		perror("Unable to fork process");
	if (!pid) {
		if (execvp(args[0], args) == -1) {
			perror("Error executing command");
		}
	}

	if (waitpid(pid, &pidstatus, 0) == -1 && pid != -1)
		perror("Error waiting for process.");
}

