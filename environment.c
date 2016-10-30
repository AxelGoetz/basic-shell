#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "environment.h"
#include "strings.h"

record* initialiseNewRecord() {
  record *newRecord = (record*)calloc(1, sizeof(record));
  newRecord->name = (char *)calloc(INIT_BUFF, sizeof(char));
  newRecord->value = (char *)calloc(INIT_BUFF, sizeof(char));
  return newRecord;
}

/**
 * Checks if a particular record exists,
 * if it does then it returns it.
 * Otherwise returns NULL.
 */
record* lookupRecord(char *name) {
  int i = 0;
  while(records[i]) {
    if(!strcmp(records[i]->name, name)) {
      return records[i];
    }
    i++;
  }
  return NULL;
}

void addNewEntryToRecords(char *name, char *value) {
  int i = 0;
  while(records[i]) {
    i++;
  }
  records[i] = initialiseNewRecord();
  strcpy(records[i]->name, name);
  strcpy(records[i]->value, value);
}

/**
 * Checks if an entry exists. If it does, it updates it.
 * Otherwise it adds a new entry.
 * @param name  [description]
 * @param value [description]
 */
void addEntryToRecords(char *name, char *value) {
  record *tempRecord = lookupRecord(name);
  if(tempRecord) {
    strcpy(tempRecord->value, value);
  } else {
    addNewEntryToRecords(name,value);
  }
}

/**
 * Processes a line and then either updates the
 * PATH, HOME or the records
 */
void processLine(char *line) {
  if(!strlen(line)) return; // Empty lines in file
  // Let's assume there cannot be another '=' in the variables
  char **parts = parseLineDelimiter(line, '=');
  if(parts[0] == NULL || parts[1] == NULL) {
    fprintf(stderr, "Each line must be in the format: NAME=VALUE\n");
  }

  if(!strcmp(parts[0], "PATH")) {
    PATH = storePath(parts[1]);
  } else if(!strcmp(parts[0], "HOME")) {
    strcpy(HOME, parts[1]);
  } else {
    addEntryToRecords(parts[0], parts[1]);
  }

  freeLines(parts);
}

/**
 * Parses the file line-by-line
 * Then for every line, it stores the variable.
 * @param file [description]
 */
void parseFile(FILE *file) {
  char buff[INIT_BUFF];
  char *line = (char *)calloc(INIT_BUFF, sizeof(char));
  int lineSize = INIT_BUFF, currentChar = 0;

  // Processing in buffers is faster
  while (fgets(buff, INIT_BUFF, file) != NULL) {
    int i = 0;
    while(buff[i]) {
      if(buff[i] == '\n') {
        line[currentChar] = '\0';
        processLine(line);
        free(line);
        line = (char *)calloc(INIT_BUFF, sizeof(char));
        lineSize = INIT_BUFF; currentChar = 0;
      } else {
        line[currentChar] = buff[i];
        currentChar++;
        char *tempString = doubleStringLength(line, currentChar, lineSize);
        if(tempString != NULL) { // String has been doubled
          line = tempString;
          lineSize *= 2;
        }
      }
      i++;
    }
  }
  line[currentChar] = '\0';
  processLine(line);
  free(line);
  if (!feof(file)) {
    printf("An error occured while reading the profile file\n");
  }
}

void freeRecords() {
  int i = 0;
  while(records[i]) {
    free(records[i]->name);
    free(records[i]->value);
    free(records[i]);
    i++;
  }
  free(records);
}

/**
 * HOME and PATH need to be assigned in profile
 * Otherwise, report an error
 */
int checkIfHomeAndPathExist() {
  if(HOME != NULL && PATH != NULL) {
    return 1;
  }
  fprintf(stderr, "PATH or HOME are not assigned. Make sure that both variables are assigned in profile\n");
  return 0;
}

/**
* Initialises the environment by initialising global variables
* and reading from the "profile" file.
* @return 0 if init fails, otherwise 1
*/
int initEnvironment() {
  HOME = (char *)calloc(INIT_BUFF, sizeof(char));
  // Let's assume we never have more than INIT_BUFF records
  records = (record **)calloc(INIT_BUFF, sizeof(record*));

  FILE *f = fopen("profile", "r");
  if(f) {
    parseFile(f);
    fclose(f);
    if(checkIfHomeAndPathExist()) {
      if(chdir(HOME)) {
        fprintf(stderr, "HOME directory not found\n");
        return 0;
      } return 1;
    } return 0;
  }
  fprintf(stderr, "Could not open profile file\n");
  free(HOME); freeRecords();
  return 0;
}

void cleanupEnvironment() {
  freeRecords();
  free(HOME);
  freeLinkedList(PATH);
}
