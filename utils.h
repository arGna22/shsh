struct arg
{
	char *cmd;
	struct arg *next;
};

struct arg *strsplit(char *input);
struct arg *create(char *text);
void getArgsArray(struct arg *args, char *arr[]);
void exeCmd(struct arg *args); 

