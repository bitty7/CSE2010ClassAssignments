/*
  Author: Phil Chan
  Email: pkc@cs.fit.edu
  Description:

  An implementation of doubly linked lists (DLL) with these operations:

  a.  dll_insert(list, key): insert a key to the list
  b.  dll_delete(list, key): delete a key from the list
  c.  dll_find(list, key): find a key and return the node or NULL
  d.  dll_print(list): print the list

  Keys in a DLL are in an ascending order.

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "skiplist.h"

//#define DEBUG



// create and return a node with data and previous/next nodes
DLL_Node* dll_create_node(int data, char *activity, DLL_Node* prev, DLL_Node* next, DLL_Node* top, DLL_Node* bottom)
{
  DLL_Node *my_node = (DLL_Node *)malloc(sizeof(DLL_Node));

  if (my_node == NULL)
    {
      printf("malloc failed, memory is full\n");
      exit(-1);
    }

  my_node->data = data;
  strcpy(my_node->activity, activity);
  my_node->prev = prev;
  my_node->next = next;
  my_node->top = top;
  my_node->bottom = bottom;


  return my_node;
}


// insert a key and maintain the ascending order of DLL
// assuming keys are unique
int put(DLL *list,  int key, char *activity, DLL_Node *new_node)
{
  // empty list
  if (list->head == NULL)
    {

      list->head = malloc(sizeof(DLL_Node));
      list->head = new_node;
    }

  // insert at head, key is smaller than the first node
  else if ( (list->head != NULL) && (key < list->head->data))
    {

      DLL_Node *first_node = list->head;  // first node of the list

      new_node->next = first_node;
      list->head = new_node;
      first_node->prev = new_node;
    }

  // insert in the middle or at the end
  else
    {
      DLL_Node *curr_node = list->head; // current node

      while ( (curr_node->next != NULL) && (key > curr_node->next->data) ){
          if (curr_node->next->data == key){
              return 1;
          }
	      curr_node = curr_node->next;
      }

      if (curr_node->next != NULL) // in the middle, between curr_node and curr_node->next
	     {
             if (curr_node->next->data == key){
                 return 1;
             }

	        new_node->prev = curr_node;
	        new_node->next = curr_node->next;
	        curr_node->next->prev = new_node;

	     }
      else // at the end, insert after curr_node
	    {
	       new_node->prev = curr_node;
	    }
      curr_node->next = new_node;
      return 0;
    }
}

/*
    given a skip list, key, and highest level of the skip list,
    this function will find the node that has the same key value in the bottom lost node, asuming all keys are unique
*/
DLL_Node* get( DLL *list, int key, int highest_level){
    DLL_Node *temp_node = NULL; // temporary pointer to use later


    for (int i = highest_level - 1; i >= 0 ; i--) { // loop through all the level of the skip list, for each level, perform the following lines
        temp_node = list[i].head; // set temp_node to be the each of the linked list at this specific level

        /*
          if linked list at this level is not empty, then go through all of its nodes to find the right key,
          when the loop stops, check if the temp_node equals key, if yes, then go the bottom most node of temp_node

        */
        if (temp_node != NULL) {

            while( (temp_node->next != NULL) && (temp_node->data < key) ) {
                temp_node = temp_node->next;
            }

            if (temp_node->data == key){
                while (temp_node->bottom != NULL) {
                    temp_node = temp_node->bottom;
                }
                break;
            }

            if ( temp_node->bottom != NULL){

                temp_node = temp_node->bottom;
            }
        }

    }

    /*
      if temp_node value equals to key, then return temp_node
      else, return NULL
    */
    if (temp_node->data == key){
        return temp_node;
    }else {
        return NULL;

    }


}


// delete a node with key and maintain the ascending order of DLL
void remove_node(DLL *list, int key, int highest_level)
{

  DLL_Node *found_node = get(list, key, highest_level); // store the value returned from get() which is the node with the value of (key)
  DLL_Node *temp_node_to_delet; // temporary pointer to hold the node we need to delete


  if (found_node != NULL) // if the found node is not empty
    {
      if ((found_node->next == NULL) && (found_node->prev == NULL)) // if it's only node, then set the linked list to be NULL
	    {
	  list->head = NULL;
        }
      else if (found_node->prev == NULL)  // if it's first node, then move change head to be the next node
	    {
	  list->head = found_node->next;
	  found_node->next->prev = NULL;
        }
      else if (found_node->next == NULL)  // if it's last node, then go to its previous node and set its next to be NULL
	    {
	  found_node->prev->next = NULL;
        }
      else // if its in the middle, or in between two nodes, then adjust the pointers of next and previous nodes
	    {
	  found_node->next->prev = found_node->prev;
	  found_node->prev->next = found_node->next;
	    }

        /*
            while found_node is not empty, set temp_node_to_delete to be found_node, then free temp_node_to_delete
            update found node to be its top pointer
            if found_node in the higher level has a previous node, then adjust the previous node pointers before deleting it
          */
      while (found_node != NULL) {

          temp_node_to_delet = found_node;
          free(temp_node_to_delet);

          found_node = found_node->top;

          if ( (found_node != NULL) && (found_node->prev != NULL) ){
              found_node->prev->next = NULL;
          }
      }

    }
  else // key was not found or list is empty
    printf("dll_delete: %d was not found or list is empty\n", key);
}

// returns the ceiling node of the node that has the key value
DLL_Node* ceilingEntry( DLL *list, int key, int highest_level) {
    DLL_Node *temp_node = NULL; // temporary variable to store node, and to use later

    /*
        for each level of the skip list, set temp_node to be the head of the linked list at that level
        then, while the next node is not NULL, and while the value of node is less than key, move forward until it stops
      */
    for (int i = highest_level - 1; i >= 0 ; i--) {
        temp_node = list[i].head;

        if (temp_node != NULL) {

            while ((temp_node->next != NULL) && (temp_node->data < key)) {
                temp_node = temp_node->next;
            }

            /*
                if temp_node value is equals to key, then move to its bottom most node.
                then break from the for loop
              */
            if (temp_node->data == key) {
                while (temp_node->bottom != NULL) {
                    temp_node = temp_node->bottom;
                }
                break;
            }
        }


    }

    /*
        if temp_node value equals to key, return key
        if
      */
    if (temp_node->data == key){
        if (temp_node->next != NULL)
            return temp_node->next;
    }else if(temp_node->next != NULL){
        return temp_node;

    }


}

//retruns the floor node of the node that has the key value
DLL_Node* floorEntry( DLL *list, int key, int highest_level){

    DLL_Node *temp_node; // temporary variable
    temp_node = get(list, key, highest_level); // store the returned value from get() in temp_node

    /*
        if temp_node is not empty, then if previous is not empty, return the previous pointer of temp_node
        else, if temp_node is empty, then while previous is not empty, set temp_node to be its previous node, then return the previous most node of temp_node
      */
    if (temp_node){
        if (temp_node->prev != NULL) {
            return temp_node->prev;
        }
    }else {
        while (temp_node->prev != NULL ){
            temp_node = temp_node->prev;
        }
        return temp_node;
    }

}

DLL_Node *submap(DLL *list, int key1, int key2, int highest_level){

    DLL_Node *first_temp_node = get(list, key1, highest_level); // find the node that has the value of key1 and store it in first_temp_node
    DLL_Node *second_temp_node = get(list, key2, highest_level); // find the node that has the value of key2 and store it in second_temp_node

    DLL_Node *temp_node; // temporary varibale

    DLL_Node *submap;// iniiate the submap

    if (first_temp_node == NULL) { // if first_temp_node is empty, then find its ceiling
        first_temp_node = ceilingEntry(list, key1, highest_level);
    }
    if (second_temp_node == NULL){ // if second_temp_node is empty, then find its floor
        second_temp_node = floorEntry(list, key2, highest_level);
    }

    /*
        if first_temp_node and second_temp_node are not empty, then
        set submap to be first_temp_node, and set temp_node to be submap
      */
    if ( (first_temp_node != NULL) && (second_temp_node != NULL)) {
      submap = first_temp_node;
      temp_node = submap;

      /*
          move until the end of temp_node, stop when the next node is NULL or when the next node's value is the same as second_temp_node value
        */
        while ( (temp_node->next != NULL) && (temp_node->next->data != second_temp_node->data)) {

            temp_node = temp_node->next;
        }

        if (temp_node->next != NULL) {// is the temp_node next is not empty, set it to be empty
          temp_node->next = NULL;
        }

        return submap;

    } else {
        return NULL;
    }

}


// --------------  testing --------------------------------
// uncomment #define DEBUG near the top


#ifdef DEBUG

// test dll_find() and print failures
void test_dll_find(DLL *list, int key)
{
  DLL_Node *found_node = dll_find(list, key);

  if (found_node == NULL || (found_node != NULL && found_node->data != key))
    printf("*** fail: %d ***\n", key);
}

// testing the operations of DLL
void test_dll()
{
  DLL mylist;
  mylist.head = NULL;

  dll_delete(&mylist, 100);  dll_print(&mylist);

  dll_insert(&mylist, 40);  dll_print(&mylist);
  dll_insert(&mylist, 20);  dll_print(&mylist);
  dll_insert(&mylist, 60);  dll_print(&mylist);
  dll_insert(&mylist, 50);  dll_print(&mylist);
  dll_insert(&mylist, 70);  dll_print(&mylist);
  dll_insert(&mylist, 30);  dll_print(&mylist);
  dll_insert(&mylist, 10);  dll_print(&mylist);


  test_dll_find(&mylist, 10);
  test_dll_find(&mylist, 50);
  test_dll_find(&mylist, 70);
  if (dll_find(&mylist, 100) != NULL)
    printf("***fail 100***\n");

  dll_delete(&mylist, 5);  dll_print(&mylist);
  dll_delete(&mylist, 55);  dll_print(&mylist);
  dll_delete(&mylist, 105);  dll_print(&mylist);

  dll_delete(&mylist, 10);  dll_print(&mylist);
  dll_delete(&mylist, 70);  dll_print(&mylist);
  dll_delete(&mylist, 40);  dll_print(&mylist);
  dll_delete(&mylist, 20);  dll_print(&mylist);
  dll_delete(&mylist, 60);  dll_print(&mylist);
  dll_delete(&mylist, 50);  dll_print(&mylist);
  dll_delete(&mylist, 30);  dll_print(&mylist);

  dll_delete(&mylist, 100);  dll_print(&mylist);
}


int main(int argc, char* argv[])
{
  test_dll();
}
#endif
