#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

// a node in DLL
typedef struct dll_node
{
  int data; // this will be the time
  char activity[50];
  struct dll_node *prev, *next, *top, *bottom;
} DLL_Node;


// DLL with only head
// Using a struct allows head to be changed by passing address of DLL,
// instead of passing address of head (double pointer),
//    which might be more confusing
// This also allows more fields to be added later if desired.
typedef struct dll
{
  DLL_Node   *head;
} DLL;


int put(DLL *list,  int key, char *activity, DLL_Node *new_node);
void remove_node(DLL *list,  int key, int highest_level);
DLL_Node* get( DLL *list, int key, int highest_level);
DLL_Node* ceilingEntry( DLL *list, int key, int highest_level);
DLL_Node* floorEntry( DLL *list, int key, int highest_level);
DLL_Node *submap(DLL *list, int key1, int key2, int highest_level);

#endif
