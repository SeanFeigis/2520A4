#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Basic functions

//Malloc and return a performance struct
struct Performance *newPerformance() {
  struct Performance *perf;
  if ((perf = malloc(sizeof(struct Performance))) == NULL) {
    fprintf(stderr, "%s\n", "Error, malloc failed");
  }
  perf->reads=0;
  perf->writes=0;
  perf->mallocs=0;
  perf->frees=0;
  return perf;
}

//Malloc and attach a node to the tree
void attachNode (struct Performance *performance, struct Node **node_ptr, void *src, unsigned int width) {

  struct Node *temp = (struct Node *) malloc(sizeof(struct Node));
   if (!temp) { //Null malloc check
     fprintf(stderr, "%s\n", "Error, malloc failed");
     exit(0);
   }
   if (!(temp->data = (void *) malloc(width))) { //Null malloc check
     fprintf(stderr, "%s\n", "Error, malloc failed");
     exit(0);
   }

   memcpy(temp->data,src,width); //Copy data to the new element

   temp->lt = NULL;
   temp->gte = NULL;

   *node_ptr = temp;

   performance->mallocs++;
   performance->writes++;

}

int comparNode (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *),void *target) {
  performance->reads++;
  return (compar(target, (*node_ptr)->data)); //returns the comparison
}


struct Node **next(struct Performance *performance, struct Node **node_ptr, int direction) {
  if (*node_ptr) {
    performance->reads++;
    if (direction < 0 ) { //Changes the child depending on the direction
      return &((*node_ptr)->lt);
    } else {
      return &((*node_ptr)->gte);
    }
  } else {
    fprintf(stderr, "%s\n", "Error, empty tree"); //Error handling
    exit(1);
  }
}



void readNode (struct Performance *performance, struct Node **node_ptr, void *dest, unsigned int width) {

  if (*node_ptr) {
    memcpy(dest,(*node_ptr)->data,width); //copy the data
    performance->reads++;
  } else {
    fprintf(stderr, "%s\n", "Error, empty tree"); //Error handling
    exit(1);
  }

}


void detachNode (struct Performance *performance, struct Node **node_ptr) {

  if (*node_ptr) {
    free((*node_ptr)->data); //frees the data
    (*node_ptr)->data = NULL;
    free(*node_ptr); //frees the data
    *node_ptr = NULL; //set the pointer to null for good practice
    performance->frees++;
  } else {
    fprintf(stderr, "%s\n", "Error, empty tree"); //Error testing
    exit(1);
  }
}


 int isEmpty( struct Performance *performance, struct Node **node_ptr) {
   if (*node_ptr) {
     return 0;
   } else {
     return 1;
   }
 }

//DERIVED FUNCTIONS



void addItem (struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *src, unsigned int width) {
  struct Node **tree_temp = node_ptr; //sets the traversing node

  while (!isEmpty(performance, tree_temp)) {
    tree_temp = next(performance, tree_temp, comparNode(performance, tree_temp, compar, src)); //traverse through the tree in the correct direction by cmp
  }
  attachNode(performance, tree_temp, src, width); //add the node to the furthest child
}

void freeTree( struct Performance *performance, struct Node **node_ptr) {
  if (!isEmpty(performance, node_ptr)) {
    freeTree(performance, next(performance, node_ptr, -1)); //go all the way to the left
    freeTree(performance, next(performance, node_ptr, 1)); //go all the way to the right
    detachNode(performance, node_ptr); //remove the node
  }
}

int searchItem( struct Performance *performance, struct Node **node_ptr, int (*compar)(const void *, const void *), void *target, unsigned int width) {
  struct Node **tree_temp = node_ptr;
  int comparVal = 0;

  while (!isEmpty(performance,tree_temp)) {
    comparVal = comparNode(performance, tree_temp, compar, target);

    if (!comparVal) { //compare it to the target
      readNode(performance, tree_temp, target, width); //read the current node
      return 1;
    }

    tree_temp = next(performance, tree_temp, comparVal); //traverse in the correct direction
  }

  return(0);
}
