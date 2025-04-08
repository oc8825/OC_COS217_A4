#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include <stddef.h>
#include "a4def.h"
#include "path.h"

/* A Node_T is a node in a File Tree */
typedef struct node *Node_T;

int Node_new(boolean isFile, Path_T oPPath, Node_T oNParent, Node_T *poNResult);

size_t Node_free(Node_T oNNode);

Path_T Node_getPath(Node_T oNNode);

boolean Node_hasChild(Node_T oNParent, Path_T oPPath,
                      size_t *pulChildID);

size_t Node_getNumChildren(Node_T oNParent);

int Node_getChild(Node_T oNParent, size_t ulChildID,
                  Node_T *poNResult);

Node_T Node_getParent(Node_T oNNode);

int Node_compare(Node_T oNFirst, Node_T oNSecond);

char *Node_toString(Node_T oNNode);

boolean Node_isFile(Node_T oNNode);

void *Node_getContents(Node_T oNNode);

#endif