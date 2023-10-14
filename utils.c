#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "utils.h"

char *get_current_directory() 
{
    long path_max = pathconf(".", _PC_PATH_MAX);
    long name_max = pathconf(".", _PC_NAME_MAX);
    char *buff = malloc(path_max);
    char *relative_path_start;
    char *relative_path = malloc(name_max);

    if (!buff) {
        perror("malloc");
        return NULL;
    }

    if (!getcwd(buff, path_max)) {
        perror("getcwd");
        free(buff);
        return NULL;
    }

    relative_path_start = strrchr(buff, '/');
    
    if (strlen(relative_path_start) > 1) relative_path_start++; 

    strcpy(relative_path, relative_path_start);

    free(buff);
    return relative_path;
}

int is_shell_operator(char *str)
{
    char *operators[] = {"&&", "||", "|", ";"};

    for (int i = 0; i < sizeof(operators) / sizeof(char*); i++) {
        if (!strcmp(str, operators[i])) {
            return 1;
        } 
    }
    return 0;
}