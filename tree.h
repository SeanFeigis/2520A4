//Author: Sean Shaya Feigis
//October 27, 2020
//Header file for A4

#ifndef _TREE_H_
#define _TREE_H_


//Structs
struct Node {
  void* data;
  struct Node *lt;
  struct Node *gte;
};

struct Performance
{
  unsigned int reads;
  unsigned int writes;
  unsigned int mallocs;
  unsigned int frees;
};

//Function Declarations
struct Performance *newPerformance();
void attachNode (struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width);
int compareNode (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *),void *target);
struct Node **next(struct Performance *performance, struct Node **node_ptr, int direction);
void readNode (struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width);
void detachNode (struct Performance *performance, struct Node **node_ptr);
void addItem (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int width);
void freeTree( struct Performance *performance, struct Node **node_ptr);
int searchItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width);


#endif /* <array.h> included */
