enum iostream {
	IN,
	OUT,
	ERR
};
struct redirInfo {
	enum iostream streams[5];
	char *filename;
	FILE *redirect;
	int len;
};

void error(char *msg);
void gethost(char *var);
int strsplit(char *str, char delim, char **strings, int len);
void runCmd(char **args, int redir, struct redirInfo *info);


