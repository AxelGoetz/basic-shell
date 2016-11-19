/**
 * Responsible for storing the PATHS and looking in them for the appropriate program
 */

#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "strings.h"
#include "path.h"

#define PATH_SEPARATOR ':'
#define DIR_SEPARATOR '/'

/**
 * @return A new linked list node with memory assigned for it
 */
node *initialiseNewNode() {
  node *newNode = (node *)calloc(1, sizeof(node));
  newNode->path = (char *)calloc(INIT_BUFF, sizeof(char));
  return newNode;
}

void printLinkedList(node *list) {
  do {
    printf("%s\n", list->path);
    list = list->next;
  } while(list);
}

/**
 * Stores the paths in a linked list.
 * Assuming that they do not contain any spaces.
 * @param paths a list with all of the paths (seperated by PATH_SEPARATOR)
 * @return the first node of a linked list where each node is one path to search
 */
node * storePath(char *paths) {
  char** path = parseLineDelimiter(paths, PATH_SEPARATOR);
  int i = 0;

  node *start = initialiseNewNode();
  node *currentNode = start;
  strcpy(currentNode->path, path[i++]);

  while(path[i]) {
    node *newNode = initialiseNewNode();
    strcpy(newNode->path, path[i++]);
    currentNode->next = newNode;
    currentNode = newNode;
  }
  currentNode->next = NULL;

  freeLines(path);
  return start;
}

void freeLinkedList(node *list) {
  do {
    node *temp = list;
    free(list->path);
    list = list->next;
    free(temp);
  } while(list);
}

/**
 * Returns a string with the full path of the program
 * @return A new string where path is appended to the program string
 */
char * getProgramPath(char *path, char *program) {
  int pathLen = strlen(path), programLen = strlen(program);
  char * fullPath = (char *)calloc(pathLen + programLen + 1, sizeof(char));
  fullPath[0] = '\0';
  strcat(fullPath, path);

  if(fullPath[pathLen - 1] != DIR_SEPARATOR) {
    fullPath[pathLen] = DIR_SEPARATOR;
    fullPath[pathLen + 1] = '\0';
  }

  strcat(fullPath, program);

  return fullPath;
}

/**
 * Looks for a program in the directories specified in PATH.
 * @param program the program name
 * @param list Linked list with all of the paths to look in
 * @return the full path of the program if it existt else NULL.
 */
char * lookForProgram(char *program, node *list) {
  DIR *dir;
  struct dirent *ent;

  do {
    if ((dir = opendir(list->path)) != NULL) {
      while ((ent = readdir(dir)) != NULL) {
        if(!strcmp(ent->d_name, program)) {
          closedir(dir);
          return getProgramPath(list->path, program);
        }
      }
      closedir(dir);
    } else {
      fprintf(stderr, "Could not open directory in PATH (%s)\n", list->path);
    }
    list = list->next;
  } while(list);
  return NULL;
}
