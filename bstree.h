#ifndef __BSTREE_H__
#define __BSTREE_H__

#include "include/bst.h"

typedef struct BST BSTree;

void init_bstree(BSTree *bstree);
void insert_bstree(BSTree *bstree, Item *i);
void traverse_bstree(BSTree *bstree, int low, int high, 
    function_on_node fun);
void delete_bstree(BSTree *bstree, key_type key);
Item* search_bstree(BSTree *bstree, key_type key);
Item* select_bstree(BSTree *bstree, int index);

#endif


