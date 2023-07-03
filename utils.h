struct arg *strsplit(char *input);
void exeCmd(struct arg *args); 
char *trimWhitespace(char *str);
int redirectOutput(struct arg *args);
void resetDescriptorTable();
void cleanArgs(struct arg *args);
