#ifndef __ITEM_H__
#define __ITEM_H__

#include "data.h" /* Need UserExtraData*/

#define true 1
#define false 0

struct TreeNode
{    
		Item *item;                 /* User`s extra information */
		struct TreeNode* left;     /* Left subtree */
		struct TreeNode* right;    /* Right subtree */
#ifdef SPLAY_TREE
    struct TreeNode* parent;   /* Parent node */
#endif
		int number_of_nodes;       /* How many nodes in this tree */
};
typedef struct TreeNode* TreeLink;  
typedef struct TreeNode TreeNode;

#define LESS(A, B) (A < B)
#define EQUAL(A, B) (!LESS(A, B) && !LESS(B, A))
#define EXCHANGE(A, B) do{Item t = A; A=B; B=t;}while(0)

#endif
