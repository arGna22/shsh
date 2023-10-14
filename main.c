#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "utils.h"

int main()
{
    // We are not having a convoluted prompt as before
    // It will simply consist of the current working directory and a > sign
    char *current_dir = get_current_directory();
    char *input = NULL;
    size_t input_size;
    int command_count = 0;
    int command_max = 2;
    char ***commands = malloc(command_max * sizeof(char**));

    char **command = NULL;
    char *token;

    int arg_count = 0;
    int arg_max = 3;

    while (1) {
        printf("%s > ", current_dir);

        getline(&input, &input_size, stdin);
        input[strcspn(input, "\n")] = '\0';

        token = strtok(input, " ");

        command = malloc(arg_max * sizeof(char*));
        arg_count = 0;
        while (token != NULL) {
            command[arg_count] = strdup(token);
            if (is_shell_operator(token)) {
                commands[command_count] = command;
                command_count++;
                arg_max = 3;
                command = malloc(arg_max * sizeof(char*));
                arg_count = 0;
                if (command_count == command_max) {
                    command_max++;
                    commands = realloc(commands, command_max * sizeof(char**));
                }
            }
            arg_count++;
            if (arg_count == arg_max) {
                arg_max++;
                command = realloc(command, arg_max * sizeof(char*));
            }
            token = strtok(NULL, " ");
        }

        // This is where command execution will take place.
        command_count = 0;
        command_max = 2;

        // How do I even free anything?
    }        
        free(current_dir);
}
