#include <stdio.h>
#include <string.h>
#include "utils.h"
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include "argslinkedl.h"

int main()
{
	FILE *f = fopen("myfile.txt", "w");
	dup2(1, 8);
	dprintf(8, "hi");
	return 0;
}
