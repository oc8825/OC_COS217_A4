/*--------------------------------------------------------------------*/
/* checkerDT.c                                                        */
/* Author:                                                            */
/*--------------------------------------------------------------------*/

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "checkerDT.h"
#include "dynarray.h"
#include "path.h"

/* see checkerDT.h for specification */
boolean CheckerDT_Node_isValid(Node_T oNNode)
{
   Node_T oNParent;
   Path_T oPNPath;
   Path_T oPPPath;

   /* Sample check: a NULL pointer is not a valid node */
   if (oNNode == NULL)
   {
      fprintf(stderr, "A node is a NULL pointer\n");
      return FALSE;
   }

   /* Sample check: parent's path must be the longest possible
      proper prefix of the node's path */
   oNParent = Node_getParent(oNNode);
   if (oNParent != NULL)
   {
      oPNPath = Node_getPath(oNNode);
      oPPPath = Node_getPath(oNParent);

      if (Path_getSharedPrefixDepth(oPNPath, oPPPath) !=
          Path_getDepth(oPNPath) - 1)
      {
         fprintf(stderr, "P-C nodes don't have P-C paths: (%s) (%s)\n",
                 Path_getPathname(oPPPath), Path_getPathname(oPNPath));
         return FALSE;
      }
   }

   return TRUE;
}

/*
   Performs a pre-order traversal of the tree rooted at oNNode.
   Returns FALSE if a broken invariant is found and
   returns TRUE otherwise.

   You may want to change this function's return type or
   parameter list to facilitate constructing your checks.
   If you do, you should update this function comment.
*/
static boolean CheckerDT_treeCheck(Node_T oNNode)
{
   size_t ulIndex;
   size_t ulIndex1;
   size_t ulIndex2;

   if (oNNode != NULL)
   {
      /* Sample check on each node: node must be valid */
      /* If not, pass that failure back up immediately */
      if (!CheckerDT_Node_isValid(oNNode))
         return FALSE;

      /* Recur on every child of oNNode */
      for (ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if (iStatus != SUCCESS)
         {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            return FALSE;
         }

         /* if recurring down one subtree results in a failed check
            farther down, passes the failure back up immediately */
         if (!CheckerDT_treeCheck(oNChild))
            return FALSE;
      }

      /* Loop through all pairs of children*/
      for (ulIndex1 = 0; ulIndex1 < Node_getNumChildren(oNNode); ulIndex1++)
      {
         for (ulIndex2 = 0; ulIndex2 < Node_getNumChildren(oNNode); ulIndex2++)
         {
            Node_T oNChild1 = NULL;
            Node_T oNChild2 = NULL;
            int iStatus1 = Node_getChild(oNNode, ulIndex1, &oNChild1);
            int iStatus2 = Node_getChild(oNNode, ulIndex2, &oNChild2);

            if (iStatus1 != SUCCESS || iStatus2 != SUCCESS)
            {
               fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
               return FALSE;
            }

            /* Check for duplicate children */
            if (ulIndex1 != ulIndex2 && Node_compare(oNChild1, oNChild2) == 0)
            {
               fprintf(stderr, "Duplicate children found\n");
               return FALSE;
            }

            /* Check for children out of lexographical order */
            if (ulIndex1 < ulIndex2 && Path_comparePath(Node_getPath(oNChild1), Node_getPath(oNChild2)) > 0)
            {
               fprintf(stderr, "Children not in lexicographic order\n");
               return FALSE;
            }
            if (ulIndex1 > ulIndex2 && Path_comparePath(Node_getPath(oNChild1), Node_getPath(oNChild2)) < 0)
            {
               fprintf(stderr, "Children not in lexicographic order\n");
               return FALSE;
            }
            if (ulIndex1 == ulIndex2 && Path_comparePath(Node_getPath(oNChild1), Node_getPath(oNChild2)) != 0)
            {
               fprintf(stderr, "Children not in lexicographic order\n");
               return FALSE;
            }
         }
      }
   }
   return TRUE;
}

/* Recursively loops through every node, adding it to the overall
count, and returns the total number of nodes found in the subtree
rooted at oNNode */
static size_t CheckerDT_countNodes(Node_T oNNode)
{
   size_t ulCount = 0;
   size_t ulIndex;

   if (oNNode != NULL)
   {
      /* Count this node */
      ulCount = 1;

      /* Recur on every child of oNNode */
      for (ulIndex = 0; ulIndex < Node_getNumChildren(oNNode); ulIndex++)
      {
         Node_T oNChild = NULL;
         int iStatus = Node_getChild(oNNode, ulIndex, &oNChild);

         if (iStatus != SUCCESS)
         {
            fprintf(stderr, "getNumChildren claims more children than getChild returns\n");
            return 0;
         }

         /* Add count of all descendents to total count */
         ulCount += CheckerDT_countNodes(oNChild);
      }
   }
   return ulCount;
}

/* see checkerDT.h for specification */
boolean CheckerDT_isValid(boolean bIsInitialized, Node_T oNRoot,
                          size_t ulCount)
{

   /* Sample check on a top-level data structure invariant:
      if the DT is not initialized, its count should be 0 and the
      root should be null */
   if (!bIsInitialized)
   {
      if (ulCount != 0)
      {
         fprintf(stderr, "Not initialized, but count is not 0\n");
         return FALSE;
      }
      if (oNRoot != NULL)
      {
         fprintf(stderr, "Not initialized, but root is not NULL\n");
         return FALSE;
      }
   }

   /* Now checks invariants recursively at each node from the root. */
   if (!CheckerDT_treeCheck(oNRoot))
   {
      return FALSE;
   }

   /* Ensure manual count of number of nodes matches ulCount */
   if (CheckerDT_countNodes(oNRoot) != ulCount)
   {
      fprintf(stderr, "Count of nodes does not match internal count variable ulCount\n");
      return FALSE;
   }

   /* Passed all invariant checks, we have a valid tree */
   return TRUE;
}
