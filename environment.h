#include "path.h"

// Stores other records (not path and home)
typedef struct record {
  char *name;
  char *value;
} record;

// Global variables to store home, path and other variables
char *HOME;
node *PATH;
record **records;

void processLine(char *line);
void cleanupEnvironment();
int initEnvironment();
