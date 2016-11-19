#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "strings.h"

// Ironically, the escape char needs to be escaped
#define ESCAPECHAR '\\'


/**
 * Used to see if the current character is escaped
 */
typedef struct escape {
  bool isEscaped;
  bool quote;
} escape;

/**
 * Initialises and assigns memory for a new escape instance
 */
escape* initialiseEscape() {
  escape *esc = (escape*)malloc(sizeof(escape));
  esc->isEscaped = false;
  esc->quote = false;

  return esc;
}

/**
* Debugger function that prints the content of lines
*/
void printLines(char **lines) {
  int i = 0;
  while(lines[i]) {
    printf("%s\n", lines[i++]);
  }
}

void freeLines(char **lines) {
  int i = 0;
  while(lines[i]) {
    free(lines[i++]);
  }
  free(lines);
}

/**
 * If the memory allocated for line is not enough,
 * it doubles it.
 * @param line The variable holding the string
 * @param currentChar Where in the string we are appending right now
 * @param size current size of the line variable
 */
char* doubleStringLength(char* line, int currentChar, int size) {
  if(size - 1 == currentChar) {
    char *newLine = (char *)realloc(line, sizeof(char) * size * 2);
    return newLine;
  }
  return NULL; // Null if string has not been doubled
  // We could potentially use a struct to hold the size and a string
  // But that it a future improvement.
}

/**
 * Makes sure that we do not leave any empty strings
 * in the parseLineDelimiter results.
 * For instance if we have "cd ", we do not want to have
 * ["cd", ""];
 * @return a boolean value that shows whether `result[currentSplit]` was ""
 */
bool isEmptyString(char **result, int currentSplit) {
  if(!strcmp(result[currentSplit], "")) {
    free(result[currentSplit]);
    result[currentSplit] = NULL;
    return true;
  }
  return false;
}

/**
 * Adds a new character to result, based on the current
 * state. It checks whether a character has been escaped
 * or whether you are in a string.
 * @return A boolean value that shows if a new character has been added to the string
 */
bool addChar(char **result, int currentSplit, int currentChar, char linei, escape *esc) {
  if(esc->isEscaped) {
    result[currentSplit][currentChar] = linei;
    esc->isEscaped = false;
    return true;
  } else if(linei == ESCAPECHAR) {
    esc->isEscaped = true;
    return false;
  } else if(esc->quote) {
    if(linei == '"') {
      esc->quote = false; return false;
    } else {
      result[currentSplit][currentChar] = linei;
      return true;
    }
  } else if(linei == '"') {
    esc->quote = true;
    return false;
  } else {
    result[currentSplit][currentChar] = linei;
    return true;
  }
}

/**
 * Splits a string up into a 2d array, which are split according to
 * a delimiter.
 */
char ** parseLineDelimiter(char *line, char delimiter) {
  // Let's assume we will never have more that INIT_BUFF arguments
  char **result = (char **)calloc(INIT_BUFF, sizeof(char*));
  result[0] = (char *)calloc(INIT_BUFF, sizeof(char));
  int i = 0, currentSplit = 0, currentChar = 0, splitSize = INIT_BUFF;
  escape *esc = initialiseEscape();

  while(line[i]) {
    if(line[i] == delimiter && !esc->isEscaped && !esc->quote) { // Go to the next split
      result[currentSplit][currentChar] = '\0';
      if(!isEmptyString(result, currentSplit)) currentSplit++;
      currentChar = 0;
      result[currentSplit] = (char *)calloc(INIT_BUFF, sizeof(char));
      splitSize = INIT_BUFF;
    } else {
      if(addChar(result, currentSplit, currentChar, line[i], esc)) {
        currentChar++;
        char *tempString = doubleStringLength(result[currentSplit], currentChar, splitSize);
        if(tempString != NULL) { // String has been doubled
          result[currentSplit] = tempString;
          splitSize *= 2;
        }
      }
    }
    i++;
  }
  result[currentSplit][currentChar] = '\0';
  isEmptyString(result, currentSplit);
  result[++currentSplit] = NULL;
  free(esc);
  return result;
}
