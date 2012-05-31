#include "splaytree.h"

static TreeLink __new_splay_tree_node(Item *i, TreeLink left,
    TreeLink right, TreeLink parent, int number_of_nodes)
{
  TreeLink x = malloc(sizeof(TreeNode));

  if (x == NULL)
  {
    fprintf(stdin, "malloc error :(\n");
    return NULL;
  }
  x->item = i;
  x->left = left;
  x->right = right;
  x->parent = parent;
  x->number_of_nodes = number_of_nodes;

  return x;
}
/* Left rotation */
static TreeLink __left_rotation(TreeLink l)
{
  TreeLink p = l->right;

  l->right = p->left;
#ifdef SPLAY_TREE
  p->left->parent = l;
  p->parent = l->parent;
  if (l == l->parent->right)
    l->parent->right = p;
  else
    l->parent->left = p;
  l->parent = p;
#endif
  l->number_of_nodes = l->left->number_of_nodes + l->right->number_of_nodes + 1;
  p->left = l;
  p->number_of_nodes = p->left->number_of_nodes + p->right->number_of_nodes + 1;

  return p;
}

/* Right rotation */
static TreeLink __right_rotation(TreeLink l)
{
  TreeLink p = l->left;

  l->left = p->right;
#ifdef SPLAY_TREE
  p->right->parent = l;
  p->parent = l->parent;
  if (l == l->parent->right)
    l->parent->right = p;
  else
    l->parent->left = p;
  l->parent = p;
#endif
  l->number_of_nodes = l->right->number_of_nodes + l->left->number_of_nodes + 1;
  p->right = l;
  p->number_of_nodes = p->right->number_of_nodes + p->left->number_of_nodes + 1;

  return p;
}

/* Initialize splay tree */
void init_splay_tree(SplayTree *splay)
{
  splay->head = (splay->terminal = __new_splay_tree_node(&nullitem, 
        NULL, NULL, NULL, 0));
  splay->head->parent = splay->terminal;
}

static TreeLink __splay_to_root(SplayTree *splay, TreeLink l)
{
  /* P is l`s parent, GP is l`s grandparent */
  TreeLink p, gp;

  if (l == splay->head)
    return l;

  while (l->parent != splay->terminal)
  {
    p = l->parent;
    gp = (p != splay->terminal) ? p->parent : p;

    /* L`s parent is root */
    if (gp == splay->terminal)
    {
      if (p->left == l)
        l = __right_rotation(p);
      else
        l = __left_rotation(p);
    }

    /* Zig-Zag case */
    if (p == gp->left && l == p->right)
    {
      l = __left_rotation(p);
      l = __right_rotation(gp);
    }
    if (p == gp->right && l == p->left)
    {
      l = __right_rotation(p);
      l = __left_rotation(gp);
    }

    /* Zig-Zig case */
    if (p == gp->left && l == p->left)
    {
      l = __right_rotation(gp);
      l = __right_rotation(p);
    }
    if (p == gp->right && l == p->right)
    {
      l = __left_rotation(gp);
      l = __left_rotation(p);
    }    
  }
  return l;
}

/* Insertion(bottom up insertion) : the newly inserted item 
 * becomes the root of the tree */
static TreeLink __insert_splay(SplayTree *splay, Item *i, Less less)
{
  TreeLink prev, next = splay->head;

  /* Empty splay tree */
  if (splay->head == splay->terminal)
  {
    return __new_splay_tree_node(i, splay->terminal,
        splay->terminal, splay->terminal, 1);
  }
  /* Basic binary search */
  while (next != splay->terminal)
  {
    prev = next;
    next->number_of_nodes += 1;
    next = less(i, next->item) ? next->left : next->right;
  }
  next = __new_splay_tree_node(i, splay->terminal,
      splay->terminal, prev, 1);
  if (next != NULL)
  {
    if (less(i, prev->item))
      prev->left = next;
    else
      prev->right = next;
  }
  return __splay_to_root(splay, next);
}

void insert_splay(SplayTree *splay, Item *i, Less less)
{
  splay->head = __insert_splay(splay, i, less);
}

/* Traverse the Splay tree, if possible, execute FUNC on each node */
static void __traverse_splay(TreeLink head, TreeLink terminal, int low, int high,
    function_on_node func)
{
  if (head->left != terminal)
    __traverse_splay(head->left, terminal, low, high, func);

  level1++;
  if (level1 >= low && level1 <= high)
  {
    if (func != NULL)
      func(head->item);
    printf("[number_of_nodes %d]\n", head->number_of_nodes);
  }
  if (level1 > high)
    return;

  if (head->right != terminal)
    __traverse_splay(head->right, terminal, low, high, func);

  return ;
}

/* Traverse the BST */
void traverse_splay(SplayTree *splay, int low, int high, function_on_node func)
{
  if (splay->head == splay->terminal)
  {
    printf("Opoos, Empty tree :( \n");
    return ;
  }
  level1 = 0;
  if (!level1)
    __traverse_splay(splay->head, splay->terminal, low, high, func);
  else
    printf("Traverse Error :(\n");
}

/* Search */
static TreeLink __search_splay(SplayTree *splay, key_type k, Item **x, 
    Key key_function)
{
  TreeLink prev, next = splay->head;

  while (next != splay->terminal)
  {
    prev = next;
    if (EQUAL(k, key_function(next->item)))
      break;
    if (LESS(k, key_function(next->item)))
      next = next->left;
    else
      next = next->right;
  }
  if (next != splay->terminal)
  {
    *x = next->item;
    return __splay_to_root(splay, next);
  }
  else
  {
    *x = NULL;
    return __splay_to_root(splay, prev);
  }
}

/* The last node accessed during the search is splayed, if the search
 * is successful, then the node that is found is splayed and becomes 
 * the root, if the search is unsuccessful, the last node accessed is
 * splayed and becomes the root */
Item *search_splay(SplayTree *splay, key_type k, Key key_function)
{
  Item *x;

  if (splay->head == splay->terminal)
  {
    fprintf(stdin, "Empty splay tree :(\n");
    return NULL;
  }

  splay->head = __search_splay(splay, k, &x, key_function);
  
  return x;
}

static TreeLink __search_splay_min_max(SplayTree *splay, Item **x, bool min_max)
{
  TreeLink next = splay->head;
  
  {
  if (min_max)
  {
    /* Max */
    while (next->right != splay->terminal)
      next = next->right;
  } else
  {
    while (next->left != terminal)
      next = next->left;
  }
  *x = next->item;
  return __splay_to_root(splay, next);
}

/* Search for min */
Item *search_splay_min(SplayTree *splay)
{
  Item *x;

  if (splay->head == splay->terminal)
  {
    fprintf(stdin, "Empty splay tree :(\n");
    return NULL;
  }

  splay->head = __search_splay_min_max(splay, &x, 0);

  return x;
}

/* Search for max */
Item *search_splay_max(SplayTree *splay)
{
  Item *x;

  if (splay->head == splay->terminal)
  {
    fprintf(stdin, "Empty splay tree :(\n");
    return NULL;
  }

  splay->head = __search_splay_min_max(splay, &x, 1);

  return x;
}

/* Delete min, perform a search min, this brings the minimum to 
 * the root, and there is no left child, use the right child as 
 * the new root*/
Item *delete_splay_min(SplayTree *splay)
{
  Item *x;
  TreeLink tmp;

  if (splay->head == splay->terminal)
  {
    fprintf(stdin, "Empty splay tree :(\n");
    return NULL;
  }
  splay->head = __search_splay_min_max(splay, &x, 0);
  tmp = splay->head;
  splay->head->right->parent = splay->head->parent;
  splay->head = splay->right;

  free(tmp);

  return x;
}

Item *delete_splay_max(SplayTree *splay)
{
  Item *x;
  TreeLink tmp;

  if (splay->head == splay->terminal)
  {
    fprintf(stdin, "Empty splay tree :(\n");
    return NULL;
  }
  splay->head = __search_splay_min_max(splay, &x, 1);
  tmp = splay->head;
  splay->head->left->parent = splay->head->parent;
  splay->head = splay->left;

  free(tmp);

  return x;
}

static TreeLink __delete_splay(SplayTree *splay, key_type k, Key key_function)
{

}

Item *delete_splay(SplayTree *splay, key_type k, Key key_function)
{
  Item *x;


}
