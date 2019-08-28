/*

  Author: Thabet Alenezi
  Email: talenezi2018@my.fit.edu
  Course: CSE2010
  Section: 1
  Description:




 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
  Description of the function, parameter, and return value (if any)
 */

/*
  a structure for making a tree
 */
typedef struct Tree{
    char category_name[40];
    struct Tree *sibling;
    struct Tree *child;
    struct Tree *parent;
}Tree;

/*
  a structure to insert elements from tree to a linked list when needed
 */
typedef struct List {
    char category_name[40];
    struct List *next;
}List;



/*
  this function is to add a new node to the Tree
  **parent_node is the node that the new node should be a child of or a sibling of
  category_name is the name of the new node

  returns a pointer to the new node
 */
Tree *add_new_node(Tree **parent_node, char category_name[40]);


/*
  this function is to put the new node as a child or a sibling
  **current is the node that we should check where the new node should be related to
  category_name is the name of the  node

 */
void add_siblings(Tree **current_node ,char category_name[40]);


/*
  this function is to decide is is the new node is a child or a sibling, and it finds where is the parent of the sibling node
  **root is root of the tree
  category_name is the name of the  node
  name_of_parent is the name of the parent that we want to add regarding it

 */
void add_child(Tree **root ,char category_name[40], char name_of_parent[40]);


/*
  this function is to initiate the tree by adding the root node first
  **root is root of the tree
  name is the name of the  root

 */
void initiat_root(Tree **root ,char name[40]);


/*
  this function is to to find anynode we want in the tree, usually the node we need will be theparent
  **root is root of the tree
  name_of_parent is the name of the node we want to find

  returns a pointer to the node when it finds it
 */
Tree *find(Tree **root, char name_of_parent[40]);


/*
  this function is to to find the direct parent for a specific node
  **root is root of the tree
  category is the name of the node we want to find

  returns a string of the name of that parent category
 */
char *getParent(char category_name[40], Tree **root);


/*
  this function is to decide if a node is a parent of another node
  **root is root of the tree
  category_name is the name of the child node
  name_of_parent is the name of the parent node

  returns an integer, 0 is false, 1 if true
 */
int is_super_cattegory(Tree **root, char name_of_parent[40], char category_name[40]);


/*
  this function is to add nodes to a inked list
  **head is head of thelinked list
  category_name is the name of the child node

  returns nothing
 */
void add_to_list(List **head, char category_name[40] );


/*
  this function is to find al the subcategories for a node
  **root is root of the tree
  ** head is the head of a linked list
  category_name is the name of the child node

  returns nothing
 */
void find_all_subcategories(Tree **root, List **head, char category_name[40]);


/*
  this function is to decide if a node is a child of another node
  **root is root of the tree
  category_name is the name of the parent node
  name_of_child is the name of the child node

  returns an integer, 0 is false, 1 if true
 */
int is_subcattegory(Tree **root, char category_name[40], char name_of_child[40]);


/*
  this function is to find all the direct subcategories for a node
  **root is root of the tree
  ** head is the head of a linked list
  category_name is the name of the child node

  returns nothing
 */
void getChildren( Tree **root, List **head, char category_name[40]);


/*
  this function is to add nodes to a inked list in a sorted manner
  **head is head of thelinked list
  category_name is the name of the child node

  returns nothing
 */
void add_and_sort_list(List **head, char category_name[40] );


/*
  this function is to find number of subcategories and return hat number
  **root is root of the tree
  **head is head of thelinked list
  category_name is the name of the child node

  returns number of subcategories
 */
int find_number_of_subcategories(Tree **root, int *number, char category_name[40]);


/*
  this function is to find the common category in the tree between two nodes in it
  **first_node is a pointer t the first node
  **second_node is a pointer to the second node

  returns the name of the common categoryy as a string
 */
char *find_common_category(Tree **first_node, Tree **second_node);



int main(int argc, char* argv[]){
    /*
    *root: a pointer to the root of the Tree
    *head: a pointer to the head of the linked list
    *root_category_name: is the name of the node tht we read as a root
    *name_of_child_or_sibling: stores the name of sibling orchild so it can add to to the tree
    string_read: to read the whole like from the file and divid it later
    data_input_file:  is a pointer ot the file
     */
    Tree *root = NULL;
    List *head = NULL;
    char *root_category_name = NULL;
    char *name_of_child_or_sibling;
    char string_read[1000];

    FILE *data_input_file = fopen(argv[1], "r");
    // FILE *data_input_file = fopen("/home/thabet/Desktop/CSE2010/assignment3/aaaaaaaaaaa.txt" , "r");



    while (!feof(data_input_file) ) {
        // get the root name here then loop

        if (feof(data_input_file)) {
            break;
        }

        /*
        the program reads the line first,  then if the tree is empty, then read the name of the root and add it to the Tree using initiate_root()
        then, read the name of it's childring and add them
         */
        fgets(string_read, 1000, data_input_file);

        if (root == NULL) {

            root_category_name = strtok(string_read, " \r\n\0");
            initiat_root(&root ,root_category_name);

            name_of_child_or_sibling = strtok(NULL, " \r\n\0");

            while (name_of_child_or_sibling != NULL) {

                add_child(&root, name_of_child_or_sibling, root_category_name);

                name_of_child_or_sibling = strtok(NULL, "\r \n\0");

            }
        } else {
            /*
            if the tree is NOT empty, then read the name of the new root and its
             children or sibling and add them to the tree using add_child()
             */


            root_category_name = strtok(string_read, " \r\n\0");

            name_of_child_or_sibling = strtok(NULL, "\r \n\0");

            while (name_of_child_or_sibling != NULL) {

                add_child(&root, name_of_child_or_sibling, root_category_name);

                name_of_child_or_sibling = strtok(NULL, "\r \n\0");

            }

        }
        /*
        iif root is empty, exit the while loop,
        then set root_category_name to be the NULL character
         */

        if (root_category_name == NULL){
            break;
        }

        *root_category_name = '\0';

    }
    // close file
    fclose(data_input_file);


    /*
      opening the new program
     */
    FILE *query_input_file = fopen(argv[2], "r");
    // FILE *query_input_file = fopen("/home/thabet/Desktop/CSE2010/assignment3/bbbbbbbb.txt" , "r");

    /*
      *request: to read the quaries from the file
      *first_element: to hold the first work after the request
      *second_element: to read the second elemnt after treading the first elemnt
      *third_elemet: to store strings if needed
      is_or_not: if ==0 then false, if ==1 then true
      number_of_suber_or_sub_categories: stores the number of subcategories< or upercategories
     */
    char *request;
    char *first_element;
    char *second_element;
    char *third_elemet;
    int is_or_not = 0;
    int number_of_suber_or_sub_categories = 0;

    /*
      *pointer_to_root:
      *second_pointer_to_root

      are temporarly nodes to use on main other than using the root of the tree
     */
    Tree *pointer_to_root;
    Tree *second_pointer_to_root;


    while (!feof(query_input_file) ) {
        /*
          initiate pointer_to_root and second_pointer_to_root to be root
          if the file is empty then exit the loop.
          read the whole line from the file,
          read request from string_read
          if request is empty then exit the loop
         */
        pointer_to_root = root;
        second_pointer_to_root = root;

        if (feof(query_input_file)) {
            break;
        }

        fgets(string_read, 1000, query_input_file);

        request = strtok(string_read, " \r\n\0");
        if ( (request == NULL) || (request[0] == '\0')){
            break;
        }

        if ( strcmp(request, "IsSupercategory") == 0){
            /*
              read the first and second elemt. then store the returned value from is_super_cattegory()
              is is_or_not==1 print yes, if is_or_not == 0 then print no
             */
            first_element = strtok(NULL, " \r\n\0");
            second_element = strtok(NULL, " \r\n\0");
            pointer_to_root = root;

            is_or_not = is_super_cattegory(&pointer_to_root, first_element, second_element );
            if (is_or_not == 1){
                printf("IsSupercategory %s %s yes\n", first_element,second_element );
            } else {
                printf("IsSupercategory %s %s no\n", first_element,second_element );
            }

        } else if ( strcmp(request, "DirectSupercategory") == 0) {
            /*
              read the first and second elemt. then print the returned string from direct_super_category()
             */
            first_element = strtok(NULL, " \r\n\0");
            second_pointer_to_root = root;
            printf("DirectSupercategory %s %s\n",first_element, getParent(first_element, &second_pointer_to_root)  );

        }else if ( strcmp(request, "AllSubcategories") == 0) {

            /*
              read the first and second elemt. set pointer_to_root to the node that has the first_element name.
              if pointer_to_root and it's child are not empty,  set pointer_to_root to be its child
             */
            pointer_to_root = root;
            first_element = strtok(NULL, " \r\n\0");
            pointer_to_root = find(&pointer_to_root, first_element);

            if (pointer_to_root != NULL){
                if (pointer_to_root->child != NULL){
                    pointer_to_root = pointer_to_root->child;
                }
            }

            /*
              find all subcategories using find_all_subcategories(), then print all the elements in the linked list
             */
            find_all_subcategories(&pointer_to_root, &head, first_element);

            printf("AllSubcategories %s", first_element);
            while(head != NULL){
                printf(" %s", head->category_name);
                head = head->next;
            }
            printf("\n");

        }else if ( strcmp(request, "ClosestCommonSupercategory") == 0){

            /*
              read the first and second elemt. set pointer_to_root to the node that has the first_element name.
              set second_pointer_to_root to be the node that has second_element name
              then, print first_element, second_element and the returned value from find_common_category()
             */
            first_element = strtok(NULL, " \r\n\0");
            second_element = strtok(NULL, " \r\n\0");

            pointer_to_root = root;
            second_pointer_to_root = root;

            pointer_to_root = find(&root, first_element);
            second_pointer_to_root = find(&root, second_element);

            printf("ClosestCommonSupercategory %s %s %s\n", first_element, second_element, find_common_category(&pointer_to_root, &second_pointer_to_root));

        }else if ( strcmp(request, "IsSubcategory") == 0){
            /*
              read the first and second elemt. set is_or_not to be the value returned from is_subcattegory
              then, if is_or_not == 1 print yes, if is_or_not== 0 print no
             */
            first_element = strtok(NULL, " \r\n\0");
            second_element = strtok(NULL, " \r\n\0");
            pointer_to_root = root;

            is_or_not = is_subcattegory(&pointer_to_root, first_element, second_element);

            if (is_or_not == 1){
                printf("IsSubcategory %s %s yes\n", first_element,second_element );
            } else {

                printf("IsSubcategory %s %s no\n", first_element,second_element );
            }

        }else if ( strcmp(request, "DirectSubcategories") == 0){
            /*
              read the first and second elemt. set head to NULL, set pointer_to_head to be the node that has first_element name
              find direct subcategories using find_direct_sub_categories()
              print the linked list that has the subcategories
             */
            head = NULL;
            pointer_to_root = root;

            first_element = strtok(NULL, " \r\n\0");
            pointer_to_root = find(&pointer_to_root, first_element);

            getChildren(&pointer_to_root, &head, first_element);

            printf("DirectSubcategories %s", first_element);
            if (head != NULL){
                while(head != NULL){
                    printf(" %s", head->category_name);
                    head = head->next;
                }
            }

            printf("\n");


        }else if ( strcmp(request, "AllSupercategories") == 0){

            /*
              read the first and second elemt. set pointer_to_root to be the node that has first_element name
              if pointer_to_root and its parent are not NULL, set pointer_to_root to its parent
              then, print first_element and all parents of pointer_to_root
             */
            pointer_to_root = root;
            first_element = strtok(NULL, " \r\n\0");
            pointer_to_root = find(&pointer_to_root, first_element);

            if(pointer_to_root != NULL){
                if(pointer_to_root->parent != NULL){
                    pointer_to_root = pointer_to_root->parent;
                }
            }


            printf("AllSupercategories %s", first_element);
            while(pointer_to_root != NULL){
                printf(" %s", pointer_to_root->category_name);
                pointer_to_root = pointer_to_root->parent;
            }
            printf("\n");

        }else if ( strcmp(request, "NumberOfAllSupercategories") == 0){

            /*
              read the first and second elemt. set pointer_to_root to be the node that has first_element name
              if pointer_to_root and its parent are not NULL, print the parent
             */
            pointer_to_root = root;
            first_element = strtok(NULL, " \r\n\0");

            pointer_to_root = find(&pointer_to_root, first_element);

            if(pointer_to_root != NULL){
                if(pointer_to_root->parent != NULL){
                    pointer_to_root = pointer_to_root->parent;
                }
            }


            /*
              while pointer_to_root is not empty, increament number_of_suber_or_sub_categories, and go to parent
              then print first_element and the number of elements
             */
            while(pointer_to_root != NULL){
                number_of_suber_or_sub_categories++;
                pointer_to_root = pointer_to_root->parent;
            }

            printf("NumberOfAllSupercategories %s %d\n", first_element, number_of_suber_or_sub_categories);
            number_of_suber_or_sub_categories = 0;


        }else if ( strcmp(request, "NumberOfAllSubcategories") == 0){
            /*
              read the first and second elemt. set pointer_to_root to be the node that has first_element name
              if pointer_to_root and its child are not NULL, go to child
              print first element and the returnd value from find_number_of_subcategories();
             */
            pointer_to_root = root;
            first_element = strtok(NULL, " \r\n\0");
            pointer_to_root = find(&pointer_to_root, first_element);

            if (pointer_to_root != NULL){
                if (pointer_to_root->child != NULL){
                    pointer_to_root = pointer_to_root->child;
                }

            }
            printf("NumberOfAllSubcategories %s %d\n", first_element, find_number_of_subcategories(&pointer_to_root, &number_of_suber_or_sub_categories, first_element));


        }
        /*
          if request is empty, break the loop
          else , reset request, pointer_to_root and second_pointer_to_root
         */

        if ( (request == NULL) || (request[0] == '\0')){
            break;
        }

        *request = '\0';
        pointer_to_root = root;
        second_pointer_to_root = root;

    }
    /*
      close file
     */
    fclose(query_input_file);

    return 0;

}


/*
  this function is to add a new node to the Tree
  **parent_node is the node that the new node should be a child of or a sibling of
  category_name is the name of the new node

  returns a pointer to the new node
 */
Tree *add_new_node(Tree **parent_node, char category_name[40]){

    /*
      malloc a new_node and set it's value to category_name, set parent to parent node
      set siblings and child to NULL , return new_node
     */
    Tree *new_node = malloc(sizeof(Tree));
    strcpy( new_node->category_name, category_name );
    new_node->parent = *parent_node;
    new_node->sibling = NULL;
    new_node->child = NULL;

    return new_node;

}


/*
  this function is to put the new node as a child or a sibling
  **current is the node that we should check where the new node should be related to
  category_name is the name of the  node

 */
void add_siblings(Tree **current_node ,char *category_name){
    /*
      *parent_nod: to hold parent node
      *temp_node: to hold current node
      *temp_node2: is also to hold current node
     */
    Tree *parent_node = *current_node;
    Tree *temp_node = *current_node;
    Tree *temp_node2 = *current_node;


    if ( temp_node == NULL) {
        return;
    }

    /*
      while temp_node is not empty go to it's sibling.
      once while loop is over , set temp_node =  the new node using add_new_node();
     */
    while ( temp_node != NULL) {
        temp_node = temp_node->sibling;
    }
    temp_node = add_new_node(&parent_node, category_name);

    /*
      if the child of parent node is empty then set it to be temp_node
      else, set temp_node2 to be its child , then loop till it hits the last sibling, then set its sibling to temp_node
     */
    if (parent_node->child == NULL){

        parent_node->child = temp_node;

    } else {
        temp_node2 = temp_node2->child;

        while(temp_node2->sibling != NULL){
            temp_node2 = temp_node2->sibling;
        }
        temp_node2->sibling = temp_node;

    }

}


/*
  this function is to decide is is the new node is a child or a sibling, and it finds where is the parent of the sibling node
  **root is root of the tree
  category_name is the name of the  node
  name_of_parent is the name of the parent that we want to add regarding it

 */
void add_child(Tree **root ,char category_name[40], char name_of_parent[40]) {
    /*
      pointer_to_root is to hold the **root prametar
     */
    Tree *pointer_to_root = *root;

    if (*root == NULL) {
        return;
    }

    /*
      if pointer_to_root has the same name as name_of_parent, then call add_siblings() using pointer_to_root
      if not then go the node and set it to pointer_to_root using find()
      then, if pointer is empty return , else, call add_siblings() using pointer_to_root
     */
    if (strcmp( pointer_to_root->category_name, name_of_parent) == 0  ) {
        add_siblings(&pointer_to_root, category_name);

    } else {
        pointer_to_root = find(&pointer_to_root, name_of_parent);

        if (pointer_to_root == NULL) {
            return;
        } else {
            add_siblings(&pointer_to_root, category_name);
        }

    }

}


/*
  this function is to initiate the tree by adding the root node first
  **root is root of the tree
  name is the name of the  root

 */
void initiat_root(Tree **root ,char name[40]){

    Tree *temp_node = malloc(sizeof(Tree));
    strcpy(temp_node->category_name, name);
    temp_node->child = NULL;
    temp_node->parent = NULL;
    temp_node->sibling = NULL;
    *root = temp_node;

}


/*
  this function is to to find anynode we want in the tree, usually the node we need will be theparent
  **root is root of the tree
  name_of_parent is the name of the node we want to find

  returns a pointer to the node when it finds it
 */
Tree *find(Tree **root, char name_of_parent[40]){
    /*
      declear pointer to hold the node that we need to return
     */
    Tree *pointer;
    /*
      if root is empty then return
     */
    if (*root == NULL){
        return *root;

    } else if(*root != NULL){
        /*
          if root is not empty then check if roor has the same name as name_of_parent: if true, set pointer to be root and return pointer
         */

        if( strcmp( (*root)->category_name, name_of_parent) == 0 ){
            pointer = *root;
            return pointer;
        }

    }

    /*
      if the currect node is not found yet, call the function find() using sibling  again and hen check if the new node is the one we need
     */
    pointer = find( &(*root)->sibling, name_of_parent);
    if(pointer != NULL){

        if( strcmp( pointer->category_name, name_of_parent) == 0 ){
            return pointer;
        }

    }

    /*
      if the currect node is not found yet, call the function find() using child  again and hen check if the new node is the one we need
     */
    pointer = find( &(*root)->child, name_of_parent);
    if(pointer != NULL){

        if( strcmp(pointer->category_name, name_of_parent) == 0 ){
            return pointer;
        }

    }
//    return pointer;
}


/*
  this function is to to find the direct parent for a specific node
  **root is root of the tree
  category is the name of the node we want to find

  returns a string of the name of that parent category
 */
char *getParent(char category_name[40], Tree **root){
    Tree *pointer_to_root = *root;
    pointer_to_root = find(&pointer_to_root, category_name);

    if(pointer_to_root != NULL){
        if(pointer_to_root->parent != NULL){
            return pointer_to_root->parent->category_name;
        } else {
            return "";
        }
    } else {
        return 0;
    }
}


/*
  this function is to decide if a node is a parent of another node
  **root is root of the tree
  category_name is the name of the child node
  name_of_parent is the name of the parent node

  returns an integer, 0 is false, 1 if true
 */
int is_super_cattegory(Tree **root, char name_of_parent[40], char category_name[40]) {
    Tree *pointer_to_root = *root;

    pointer_to_root = find(&pointer_to_root, category_name);

    if (pointer_to_root != NULL){
        if (pointer_to_root->parent != NULL){
            pointer_to_root = pointer_to_root->parent;

            if ( strcmp(pointer_to_root->category_name, name_of_parent ) == 0){
                return 1;
            } else {
                return  0;
            }
        } else {
            return 0;
        }

    } else {
        return 0;
    }

}


/*
  this function is to add nodes to a inked list
  **head is head of thelinked list
  category_name is the name of the child node

  returns nothing
 */
void add_to_list(List **head, char category_name[40] ){

    List *pointer_to_head = *head;
    List *temp_to_insert = malloc(sizeof(List));
    strcpy(temp_to_insert->category_name, category_name);
    temp_to_insert->next = NULL;

    if ((*head == NULL) || ( (strcmp((*head)->category_name, category_name) >= 0) )){
        temp_to_insert->next = *head;
        *head = temp_to_insert;
    } else {

        while(pointer_to_head->next != NULL) {
            pointer_to_head = pointer_to_head->next;
        }
        pointer_to_head->next = temp_to_insert;
    }
}


/*
  this function is to find al the subcategories for a node
  **root is root of the tree
  ** head is the head of a linked list
  category_name is the name of the child node

  returns nothing
 */
void find_all_subcategories(Tree **root, List **head, char category_name[40]){

    if (*root == NULL){
        return;
    }

    find_all_subcategories(&(*root)->sibling, &(*head), category_name);
    if(*root != NULL){
        add_to_list( &(*head), (*root)->category_name);
    }

    find_all_subcategories( &(*root)->child, &(*head), category_name);
}


/*
  this function is to decide if a node is a child of another node
  **root is root of the tree
  category_name is the name of the parent node
  name_of_child is the name of the child node

  returns an integer, 0 is false, 1 if true
 */
int is_subcattegory(Tree **root, char category_name[40], char name_of_child[40]) {
    Tree *pointer_to_root = *root;

    pointer_to_root = find(&pointer_to_root, category_name);
    pointer_to_root = pointer_to_root->child;

    while( pointer_to_root != NULL){

        if ( strcmp(pointer_to_root->category_name, name_of_child ) == 0){
            return 1;
        }
        pointer_to_root = pointer_to_root->sibling;
    }
    return 0;
}


/*
  this function is to find all the direct subcategories for a node
  **root is root of the tree
  ** head is the head of a linked list
  category_name is the name of the child node

  returns nothing
 */
void getChildren( Tree **root, List **head, char category_name[40]){
    Tree *pointer_to_root = *root;

    if ( *root == NULL){
        return;
    }

    getChildren( &pointer_to_root->child, &(*head), category_name);
    if(pointer_to_root != NULL){
        if (pointer_to_root->child != NULL){
            pointer_to_root = pointer_to_root->child;
            while (pointer_to_root != NULL) {

                add_and_sort_list( &(*head), pointer_to_root->category_name);

                pointer_to_root = pointer_to_root->sibling;
            }
        }

        return;
    }



}


/*
  this function is to add nodes to a inked list in a sorted manner
  **head is head of thelinked list
  category_name is the name of the child node

  returns nothing
 */
void add_and_sort_list(List **head, char category_name[40] ){

    List *pointer_to_head = *head;
    List *temp_to_insert = malloc(sizeof(List));
    strcpy(temp_to_insert->category_name, category_name);
    temp_to_insert->next = NULL;

    if ((*head == NULL) || ( (strcmp((*head)->category_name, category_name) >= 0) )){
        temp_to_insert->next = *head;
        *head = temp_to_insert;
    } else {

        while((pointer_to_head->next != NULL) && ( (strcmp(pointer_to_head->category_name, category_name) <= 0) ) ) {
            pointer_to_head = pointer_to_head->next;
        }


        if( (strcmp(pointer_to_head->category_name, category_name) > 0) ){
            temp_to_insert->next = pointer_to_head->next;
            pointer_to_head->next = temp_to_insert;
        }else if(pointer_to_head->next == NULL){
            pointer_to_head->next = temp_to_insert;
        }

    }
}


/*
  this function is to find number of subcategories and return hat number
  **root is root of the tree
  **head is head of thelinked list
  category_name is the name of the child node

  returns number of subcategories
 */
int find_number_of_subcategories(Tree **root, int *number, char category_name[40]){

    if (*root == NULL){
        return *number;
    }

    find_number_of_subcategories(&(*root)->sibling, &(*number), category_name);
    if(*root != NULL){
        (*number)++;
    }

    find_number_of_subcategories( &(*root)->child, &(*number), category_name);

    return *number;

}


/*
  this function is to find the common category in the tree between two nodes in it
  **first_node is a pointer t the first node
  **second_node is a pointer to the second node

  returns the name of the common categoryy as a string
 */
char *find_common_category(Tree **first_node, Tree **second_node){

    Tree *pointer_to_first_node = *first_node;
    Tree *pointer_to_second_node = *first_node;

    if ( (pointer_to_first_node != NULL) && (pointer_to_second_node != NULL ) ){

        if (pointer_to_first_node->parent != NULL) {

            while(pointer_to_first_node->parent != NULL){

                pointer_to_second_node = *second_node;
                if (pointer_to_second_node->parent != NULL) {

                    while(pointer_to_second_node->parent != NULL){

                        if ( strcmp(pointer_to_first_node->category_name, pointer_to_second_node->category_name) == 0  ){

                            printf("%s", pointer_to_first_node->category_name);
                            return pointer_to_first_node->category_name;

                        }
                        pointer_to_second_node = pointer_to_second_node->parent;
                    }



                }
                pointer_to_first_node = pointer_to_first_node->parent;

            }

        } else {
            return pointer_to_first_node->category_name;
        }

    }
    return pointer_to_first_node->category_name;

}
