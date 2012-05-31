#ifndef __SPLAY_H__
#define __SPLAY_H__

#include "include/bst.h"

typedef struct BST SplayTree;
static int level1 = 0;

void init_splay_tree(SplayTree *splay);
void insert_splay(SplayTree *splay, Item *i, Less less);
void traverse_splay(SplayTree *splay, int low, int high,
    function_on_node func);
Item *search_splay(SplayTree *splay, key_type k, Key key_function);
#endif
