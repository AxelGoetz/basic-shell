#define INIT_BUFF 256 // The initial size of the memory allocated

char* doubleStringLength(char* line, int currentChar, int size);
char ** parseLineDelimiter(char *line, char delimiter);
void freeLines(char **lines);
