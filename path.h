// Define a linked list to store all of the paths
typedef struct node {
  char *path;
  struct node *next;
} node;

char * lookForProgram(char *program, node *list);
void freeLinkedList(node *list);
node * storePath(char *paths);
