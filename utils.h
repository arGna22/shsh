void error(char *msg, int end);
char *trimWhitespace(char *str);
struct arg *strsplit(char *input);
void exeCmd(struct arg *args);
int redirectOutput(struct arg *args);
void cleanArgs(struct arg **args);
void resetDescriptorTable();
