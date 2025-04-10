/* nodeFT.h */
/* Ben Zhou and Owen Clarke */
/* Some contents are modified from nodeDT.h by Christoper Moretti */

#ifndef NODE_INCLUDED
#define NODE_INCLUDED

#include <stddef.h>
#include "a4def.h"
#include "path.h"

/* A Node_T is a node in a File Tree   */
typedef struct node *Node_T;

/*
 Creates a new node in the File Tree, with path oPPath and
 parent oNParent. Returns an int SUCCESS status and sets *poNResult
 to be the new node if successful. The Node object can be either a
 directory or a file, in which if it is a file the boolean isFile is
 set to TRUE, it has no children, and its attributes relating to its
 contents (oNContents) and size (contentSize) are set accordingly.
 Otherwise, the opposite happens, its file status is set to FALSE, its
 children array is allocated in memory, and anything relating to its
 contents is set to FALSE. If it fails, then sets *poNResult to NULL
 and returns status:
 * MEMORY_ERROR if memory could not be allocated to complete request
 * CONFLICTING_PATH if oNParent's path is not an ancestor of oPPath
 * NO_SUCH_PATH if oPPath is of depth 0
                or oNParent's path is not oPPath's direct parent
                or oNParent is NULL but oPPath is not of depth 1
 * ALREADY_IN_TREE if oNParent already has a child with this path
*/
int Node_new(Path_T oPPath, Node_T oNParent,
             Node_T *poNResult, boolean isFile, void *oNContents,
             size_t contentSize);

/*
 Destroys and frees all memory allocated for the subtree rooted at
 oNNode, i.e., deletes this node and all its descendents only if it is
 a directory. If it is a file, just delete the file. Returns the
 number of nodes deleted.
*/
size_t Node_free(Node_T oNNode);

/* Returns the path object representing oNNode's absolute path. */
Path_T Node_getPath(Node_T oNNode);

/*
 Returns TRUE if oNParent has a child with path oPPath. Returns
 FALSE if it does not. Automatically returns FALSE if the Node is a
 file. If oNParent has such a child, stores in *pulChildID the child's
 identifier (as used in Node_getChild). If oNParent does not have
 such a child, stores in *pulChildID the identifier that such a
 child _would_ have if inserted.
*/
boolean Node_hasChild(Node_T oNParent, Path_T oPPath,
                      size_t *pulChildID);

/* Returns the number of children that oNParent has, 0 automatically
it is a file */
size_t Node_getNumChildren(Node_T oNParent);

/*
If the Node is a file, returns NOT_A_DIRECTORY, else behaves
accordingly:
Returns an int SUCCESS status and sets *poNResult to be the child
 node of oNParent with identifier ulChildID, if one exists.
 Otherwise, sets *poNResult to NULL and returns status:
 * NO_SUCH_PATH if ulChildID is not a valid child for oNParent
*/
int Node_getChild(Node_T oNParent, size_t ulChildID,
                  Node_T *poNResult);

/*
 Returns the parent node of oNNode.
 Returns NULL if oNNode is the root and thus has no parent.
*/
Node_T Node_getParent(Node_T oNNode);

/*
 Compares oNFirst and oNSecond lexicographically based on their paths.
 Returns <0, 0, or >0 if onFirst is "less than", "equal to", or
 "greater than" oNSecond, respectively.
*/
int Node_compare(Node_T oNFirst, Node_T oNSecond);

/*
 Returns a string representation for oNNode, or NULL if
 there is an allocation error.


 Allocates memory for the returned string, which is then owned by
 the caller!
*/
char *Node_toString(Node_T oNNode);

/*
 Returns a TRUE if oNNode is a file, and FALSE if not
*/
boolean Node_isFile(Node_T oNNode);

/*
Returns the contents of the ONNode, will be NULL if oNNode is not a
file
*/
void *Node_getContents(Node_T oNNode);

/*
 Returns the size of the contents in ONNode, will be NULL if oNNode is
 not a file
*/
size_t Node_getContentsSize(Node_T oNNode);

/*
Returns the old contents of the Node and sets the new contents of the
Node according to newContents and newContentsLength. Will return NULL
if oNNode is not a file.
*/
void *Node_setContents(Node_T oNNode, void *newContents,
                       size_t newContentsLength);

/*
 Changes the isFile boolean of a oNNode, setting it to TRUE.
*/
void Node_setIsFile(Node_T oNNode);

#endif