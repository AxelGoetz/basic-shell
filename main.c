#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "environment.h"
#include "execute.h"
#include "strings.h"

#define MAX_INPUT 1024

int main() {
  if(!initEnvironment()) {
    fprintf(stderr, "Intialisation failed...\n");
    return 0;
  }

  char **args;
  char line[MAX_INPUT];

  do {
    char cwd[INIT_BUFF];
    getcwd(cwd, sizeof(cwd));
    printf("%s>", cwd);
    if (fgets(line, sizeof(line), stdin)) {
      strtok(line, "\n");
      args = parseLineDelimiter(line, ' ');
    } else {
      fprintf(stderr, "Your input failed\n"); break;
    }
  } while(executeProgram(PATH, args));

  printf("Bye...\n");
  cleanupEnvironment();
  return 0;
}
