/* User data information */

#ifndef __DATA_H__
#define __DATA_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define SPLAY_TREE
struct UserExtraData
{
  /* Just a example of UserExtraData */
		int age;     
		char *name;
		void *ptr;
};

/* When you want to include any extra information,
 * use typedef here */
typedef struct UserExtraData Item;
typedef int bool;

typedef int key_type;
/* You should provide us a macro to find the key value
 * in your UserExtraData */
#define KEY(i) (i->age)   /* Example for this UserExtraData */
static Item nullitem = {32768, "\0", NULL};

/* When we traverse the tree, we can do something on each node */
typedef void function_on_node(Item *i);
typedef bool Less(Item *a, Item *b); 
typedef bool Equal(Item *a, Item *b);
typedef void Replace(Item *a, Item *b);
typedef key_type Key(Item *a);

#endif
