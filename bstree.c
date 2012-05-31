#include "bstree.h"


static void  __traverse_bstree(TreeLink head, TreeLink terminal, int low, int high,
    function_on_node func);
static TreeLink __new_tree_node(Item *i, TreeLink left,
    TreeLink right, int number_of_nodes);
static bool  __insert_bstree(BSTree *bstree,  Item *i);
static TreeLink __search_bstree(BSTree *bstree, key_type key);
static TreeLink __in_order_predecessor(TreeLink l, TreeLink *prev,
    TreeLink terminal);
static void __delete_bstree(BSTree *bstree, key_type key);
static void __delete_node(TreeLink l, TreeLink prev, BSTree *bstree);
static void __do_actual_delete(TreeLink l, TreeLink prev, BSTree *bstree);
static TreeLink __select_bstree(BSTree *bstree, int index);


static unsigned int level = 0;
/* Return a new node */
static TreeLink __new_tree_node(Item *i, TreeLink left,
    TreeLink right, int number_of_nodes)
{
  TreeLink t = (TreeLink) malloc(sizeof(TreeNode));
  if (t == NULL)
    return NULL;
  t->item = i;
  t->left = left;
  t->right = right;
  t->number_of_nodes = number_of_nodes;

  return t;
}

/* Insert a node in BST */
static bool __insert_bstree(BSTree *bstree,  Item *i)
{
  key_type key = KEY(i);
  key_type v;
  TreeLink prev, next = bstree->head;

  if (bstree->head == bstree->terminal)
  {
    bstree->head = __new_tree_node(i, bstree->terminal, bstree->terminal, 1);
    return (bstree->head == NULL) ? false : true;
  }

  while (next != bstree->terminal)
  {
    prev = next;
    v = KEY(next->item);
    next->number_of_nodes += 1;
    next = LESS(key, v) ? next->left : next->right;
  }
  next = __new_tree_node(i, bstree->terminal, bstree->terminal, 1);
  if (next == NULL)
    return false;
  if (LESS(key, KEY(prev->item)))
    prev->left = next;
  else
    prev->right = next;

  return true;
}

/* Initialize the Binary search tree, when it was an
 * empty tree, HEAD and TERMINAL pointed to the same
 * node.
 */
void init_bstree(BSTree *bstree)
{
  bstree->head = (bstree->terminal = __new_tree_node(&nullitem,
        NULL, NULL, 0));
}

/* Insert an item with given value KEY(i) in BST */
void insert_bstree(BSTree *bstree, Item *i)
{
  bool success;
  success = __insert_bstree(bstree, i); 
  if (!success)
    printf("No enough memory, malloc error\n");
}

/* Traverse the BST, if possible, execute FUNC on each node */
static void __traverse_bstree(TreeLink head, TreeLink terminal, int low, int high,
    function_on_node func)
{
  if (head->left != terminal)
    __traverse_bstree(head->left, terminal, low, high, func);

  level++;
  if (level >= low && level <= high)
  {
    if (func != NULL)
      func(head->item);
  }
  if (level > high)
    return;

  if (head->right != terminal)
    __traverse_bstree(head->right, terminal, low, high, func);

  return ;
}

/* Traverse the BST */
void traverse_bstree(BSTree *bstree, int low, int high, function_on_node func)
{
  if (bstree->head == bstree->terminal)
  {
    printf("Opoos, Empty tree :( \n");
    return ;
  }
  level = 0;
  if (!level)
    __traverse_bstree(bstree->head, bstree->terminal, low, high, func);
  else
    printf("Traverse Error :(\n");
}

/* Find L`s in-order predecessor node */
static TreeLink __in_order_predecessor(TreeLink l, TreeLink *prev, TreeLink terminal)
{
  *prev = l;

  l = l->right;
  while (l->left != terminal)
  {
    *prev = l;
    l->number_of_nodes -= 1;
    l = l->left;
  }

  l->number_of_nodes += 1;
  return l;
}

/* Leaf node or a node only one child will be deleted here */
static void __do_actual_delete(TreeLink l, TreeLink prev, BSTree *bstree)
{
  TreeLink terminal = bstree->terminal;

  if (l->left == terminal && l->right == terminal)
  {
    if (l == prev && l == bstree->head)
    {
      bstree->head = bstree->terminal;
      goto done;
    }
    /* Here, deleting a leaf node, just remove it */
    if (LESS(KEY(l->item), KEY(prev->item)))
      prev->left = terminal;
    else
      prev->right = terminal;
  } else
  {

    if (l == prev && l == bstree->head)
    {
      bstree->head = (l->left != terminal) ? l->left : l->right;
      goto done;
    }
    /* Here, deleting a node with one child, remove the node
     * and replace it with its child */
    if (l->left != terminal)
    {
      if (prev->right == l)
        prev->right = l->left;
      else
        prev->left = l->left;
    } else
    {
      if (prev->right == l)
        prev->right = l->right;
      else
        prev->left = l->right;
    }
  }

done:
  free(l->item);
  free(l);
}

/* Delete this node with link L*/
static void __delete_node(TreeLink l, TreeLink prev, BSTree *bstree)
{
  TreeLink saved_prev, in_order_pre;
  TreeLink terminal = bstree->terminal;

  /* #### */
  if (l->right != terminal && l->left != terminal)
  {
    /* Here, deleting a node with two child, replace it with its 
     * in-order successor or its in-order predecessor node */
    in_order_pre = __in_order_predecessor(l, &saved_prev, terminal);
    *(l->item) = *(in_order_pre->item);
    l->number_of_nodes -= 1;
    __do_actual_delete(in_order_pre, saved_prev, bstree);
  }
  else
    __do_actual_delete(l, prev, bstree);
}

/* Delete a node in BST with given value KEY */
static void __delete_bstree(BSTree *bstree, key_type key)
{
  TreeLink prev, next = bstree->head;

  prev = next;
  while(next != bstree->terminal)
  {
    if (EQUAL(key, KEY(next->item)))
      break;
    prev = next;
    next->number_of_nodes--;
    next = LESS(key, KEY(next->item)) ? next->left : next->right;
  }

  printf("Node information : number_of_nodes %d, item : %d\n", next->number_of_nodes,
      next->item->age);

  if (next != bstree->terminal)
  {
    __delete_node(next, prev, bstree);
  } else
    printf("No such item to delete :(\n");
}

/* Delete a node in BST with given value KEY */
void delete_bstree(BSTree *bstree, key_type key)
{
  if (bstree->head == bstree->terminal)
  {
    printf("Opoos, Empty tree :( \n");
    return ;
  }

  __delete_bstree(bstree, key);
  return ;
}

/* Searching */
static TreeLink __search_bstree(BSTree *bstree, key_type key)
{
  TreeLink next = bstree->head;

  while (next != bstree->terminal)
  {
    if (EQUAL(key, KEY(next->item)))
      break;
    next = LESS(key, KEY(next->item)) ? next->left : next->right;
  }
  if (next == bstree->terminal)
    next = NULL;

  return next;
}

/* Searching for an item with given value KEY */
Item* search_bstree(BSTree *bstree, key_type key)
{
  TreeLink p;

  if (bstree->head == bstree->terminal)
  {
    printf("Opoos, Empty tree :( \n");
    return NULL;
  }

  return ((p = __search_bstree(bstree, key)) != NULL) ? p->item : NULL;
}

/* Select operation */
static TreeLink __select_bstree(BSTree *bstree, int index)
{
  TreeLink next = bstree->head;
  int k;

  while (next != bstree->terminal)
  {
    k = (next->left != bstree->terminal) ? next->left->number_of_nodes : 0;
    if (k + 1 == index)
      return next;
    if (k + 1 > index)
      next = next->left;
    if (k + 1 < index)
    {
      next = next->right;
      index = index - k - 1;
    }
  }

  return NULL;
}

/* Select */
Item* select_bstree(BSTree *bstree, int index)
{
  TreeLink i;

  if (bstree->head == bstree->terminal)
  {
    fprintf(stdin, "Empty tree :(\n");
    return NULL;
  }

  i = __select_bstree(bstree, index);
  return (i != NULL) ? i->item : NULL;
}
