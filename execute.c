#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "environment.h"
#include "strings.h"
#include <sys/wait.h>

/**
 * Finds and executes the program
 * @param path A linked list with paths to search
 * @param args an array of strings with all of the arguments (where the first one is the program)
 */
void executeProgramHelper(node *path, char **args) {
  char *program = args[0];
  program = lookForProgram(program, path);
  if(program == NULL) {
    fprintf(stderr, "Command not found\n");
    return;
  }

  pid_t pid = fork();
   if (pid < 0) {
     fprintf(stderr, "Fork failed. Could not execute your command\n");
   }
   else if (pid == 0) {
     execv(program, args);
   }
   else {
     waitpid(pid, NULL, 0);
   }

   free(program);
}

/**
 * Checks if the program is a 'cd', a new variable assignment
 * Or another program. Then takes the appropriate actions.
 * @param args The first argument should be the program
 * @return 1 if the program should continue executing and 0 otherwise
 */
int executeProgram(node *path, char **args) {
  char *program = args[0];
  if(!strcmp(program, "cd")) {
    if(!args[1]) {
      args[1] = (char *)calloc(INIT_BUFF, sizeof(char));
      strcpy(args[1], HOME);
    } else if(args[1][0] == '~') {
      strcpy(args[1], HOME);
    }
    if(chdir(args[1])) {
      fprintf(stderr, "Directory not found\n");
    }
  } else if(program[0] == '$') {
    processLine(program + 1);
  } else if(!strcmp(program, "printenv")) {
    printEnv();
  } else if(!strcmp(program, "exit")) {
    freeLines(args);
    return 0;
  } else if(program[0] != '\n') { // New line should do nothing
    executeProgramHelper(path, args);
  }
  freeLines(args);
  return 1;
}
