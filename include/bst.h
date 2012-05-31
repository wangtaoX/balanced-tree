#ifndef __BST_H__
#define __BST_H__

#include "treenode.h"

struct BST
{
  TreeLink head;        /* Root node */
  TreeLink terminal;    /* Sentinel node for leaf node */
};

#endif
