#include <stdio.h>
#include <string.h>
#include "utils.h"

int main()
{

	// It just spams the letter I. I have no idea why.
	char input[] = "cd";

	struct arg *arguments = strsplit(input);
	
	struct arg *current = arguments;
	
	char *word = arguments->cmd;
	while (*word)
	{
		printf("%d\n", *word);
		word++;
	}

	// progress! Now it just segfaults! It runs an extra time, meaning that the last thing is not pointing to null.
	// Interestingly, it runs an extra time.


	return 0;
}
