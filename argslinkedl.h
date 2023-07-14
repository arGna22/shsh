#define ARGSLIMIT 20
#define ARGSLENLIMIT 4096 

struct arg
{
	char *cmd;
	struct arg *next;
};
struct arg *create(char *text);
void getArgsArray(struct arg *args, char *arr[]);
void freeList(struct arg **args); 
