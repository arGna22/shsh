/**
 * Purpose: A set of functions for peroforming basic operations on 2D and 3D dynamic arrays 
 * Author: Linus Kundur-Zourntos
 * Date: 9 October 2023 
 */
#include <string.h>

void append2D(char ***command, char *argument, int *arg_count, int *arg_max)
{
    if (*arg_count + 1 > *arg_max) {
        *arg_max = *arg_count + 1; 
        *command = realloc(*command, *arg_max);
    }

    *command[*arg_count] = strdup(argument);
    *command[++(*arg_count)] = NULL;
}

void append3D(char ****commands, char **command, int *command_count, int *command_max)
{
    if (*command_count + 1 > *command_max) {
        *command_max = *command_count + 1;
        *command = realloc(*commands, *command_max);
    }
    *command[*command_count] = strdup(argument);
    *command[++(*arg_count)] = NULL;
}
