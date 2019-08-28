/*

  Author: Thabet Alenezi
  Email: talenezi2018@my.fit.edu
  Course: cse 2010
  Section: 14
  Description: This program is to use graph to implement users actions of removing friends or adding other friends

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


/*
  structure to hold the information of the users' friends
 */
typedef struct Connections{
    char user_name[30];
    struct Connections *next;

}Connections;

/*
  structure to hold users' information
 */
typedef struct Vertices{
    char user_name[30];
    Connections *connections;
    struct Vertices *next;
    struct Vertices *prev;
} Vertices;


/*
  a stack to hold the path between to users
 */
typedef struct Stack{
    char user_name[30];
    struct Stack *next;
    struct Stack *prev;
}Stack;


/*
  structure to hold the information of the visited nodes in finding the path process
 */
typedef struct Visited{
    char name[30];
    struct Visited *next;
}Visited;



/*
  this function add users to the structure Vertices
    **head_of_users: the head of Vertices
    *user_name : the user we want to add
    *connection_name: the name of the user that we want to be connected to the added user

    this function returns nothing
 */
void add_user(Vertices **head_of_users, char *user_name, char *connection_name);

/*
  this function add users to the structure Connections
    **head_of_connections: the head of connecion of a specific user
    *connection_name: the name of the user that we want to be connected to that specific user

    this function returns ONE if the user is not added, or ZERO of the user is added
 */
int add_connection(Connections **head_of_connections, char *connection_name);


/*
  this function remove users from the structure Connections
    **head_of_connections: the head of connecion of a specific user
    *connection_name: the name of the user that we want to be removed from that specific user connections

    this function returns ONE if the user is not removed, or ZERO of the user is removed
 */
int remove_connection(Connections **head_of_connections, char *connection_name);


/*
  this function add a node to the Stack
    **path: the head of the stack
    *size: the current size of the stack

    this function returns nothing
 */
void push(Stack **path, char *name, int *size);


/*
  this function remove a node from the Stack
    **path: the head of the stack
    *size: the current size of the stack

    this function returns nothing
 */
void pop(Stack **path, int *size);


/*
  this function add users to the structure Connections
    **visited: the head of visited linked list
    *name: the name of the node that we want to add to the liked list Visited

    this function returns nothing
 */
void add_to_visited(Visited **visited, char *name);


/*
  this function is to find if the user is in the list of not
    **visited: the head of visited linked list
    *name: the name of the node that we want to find in the liked list Visited

    this function returns ONE if the node is found, and ZERO if the node is not found
 */
int is_visited(Visited **visited, char *name);


/*
  this function is to find a user in the structure Vertices
    **users: the head of Vertices
    *user_to_find: the name of the node that we want to find in the liked list Visited

    this function returns a pointer to the node if found, if node is not found it will return NULL
 */
Vertices *find_user(Vertices **users, char *user_to_find);


/*
  this function is to find a user in the structure Vertices
    **users: the pointer to a specific node on the structure Vertices
    *start: the name of the starting node or user
    *target: the targeted user that we want to connect with
    **path: a stack that hold the path from *start to *target if there is any
    **visited: a structure that keep track of what we have visited
    *size: the size of the stack

    this function returns a pointer to the stack if there is connection between the start user in the target user,
    if there is no path between start and target, then it returns NULL
 */
Stack *find_path(Vertices **users, char *start, char *target, Stack **path, Visited **visited, int *size);

/*
  this function remove users from the structure Visited
    **visited: the head of visited linked list
    *name: the name of the node that we want to remove from the liked list Visited

    this function returns nothing
 */
void remove_from_visited(Visited **visited, char *name);


int main(int argc, char* argv[])
{


    Vertices *head_of_users = NULL;         // head pointer to Vertices
    Stack *path = NULL;                     // the top of the the stack
    Visited *visited_nodes = NULL;          // head of Visited
    Vertices *temp_ponter_to_users = NULL;  // temporary  pointer to use inside main

    char user_name[30];         // to store the first name after the comand
    char connection_name[30];   // to store the connection name which is
    char command[30];           // to store the command at the beginning of each line of the action file
    int number_of_lines;        // to hold the number of lined in the users/friendships file
    int stack_size = 0;         // to store the size of stack



    FILE *input_users = fopen(argv[1], "r");   // a pointer to the file or users/friendships
    FILE *input_actions = fopen(argv[2], "r"); // a pointer to the actions file

    /*
     reading the number of lines and storing it in number_of_lines variable
     then, loop through all lines and add users and their connections
    */
    fscanf(input_users, "%d", &number_of_lines);
    for (int i = 0; i < number_of_lines ; i++) {
        fscanf(input_users, "%s %s", user_name, connection_name);
        add_user(&head_of_users, user_name, connection_name);
        add_user(&head_of_users, connection_name, user_name);

    }


    fclose(input_users); // closing the users/friendship file

    while(!feof(input_actions)){ // while the file is not empty


        /*
         loop though each line of the file and scan the first work of the file and store it in command
         then, compare it to the possible commands in the file
        */
        fscanf(input_actions, "%s", command);

        if ( strcmp(command, "WantToBefriend") == 0){ // this one is to fix
            char names_visited[10][30]; // an array of strings to hold the visited nodes
            int name_number = 0;        // a counter to keep track of how many nodes havebeen visited

            /*
             read the user_name and connection_name from the file, then print them to the screen to prepare for the rest of the information
             then set visited_nodes to be empty and store the value of find_path() in the stack path
            */
            fscanf(input_actions, "%s %s", user_name, connection_name);
            printf("WantToBefriend %s %s\n",user_name, connection_name );
            visited_nodes = NULL;
            path = find_path(&head_of_users, user_name, connection_name, &path, &visited_nodes, &stack_size );

            if (path != NULL){ // if path is not empty
                printf("- Length of the shortest path: %d", stack_size);

                visited_nodes = NULL; // reset visited_nodes to prepare for the loop

                while (path != NULL){ // loop untill path is empty
                    printf("\n");

                    if (path != NULL){ // if path is not empt, then enter

                        if (stack_size > 0) { // if stack size if more than zero then enter

                            while (path->next != NULL){ // loop untill the end of the stack is reached
                                path = path->next;
                            }

                            /*
                             if stack size equals 3 then it's an intermediate friend, if less then three then it's a mutual friend
                            */
                            if (stack_size == 3){
                                printf("- Your intermediate friend is %s\n", path->prev->user_name);

                            }else {
                                printf("- Your mutual friend is %s\n", path->prev->user_name);
                            }

                            /*
                             add the intermediate/ mutual friend to visitednodes
                            */
                            strcpy(names_visited[name_number], path->prev->user_name);
                            name_number++;
                            visited_nodes = NULL;

                            for (int i = 0; i <= name_number ; i++) {
                                add_to_visited(&visited_nodes, names_visited[i]);
                            }

                            /*
                             print the path
                            */
                            printf("- path:");
                            while (path != NULL) {
                                printf("%s ", path->user_name);
                                path = path->prev;
                            }

                        }

                    }
                    stack_size = 0;
                    path = find_path(&head_of_users, user_name, connection_name, &path, &visited_nodes, &stack_size );
                }

            }else { // if path is empty from the first time then print that there is no way these two can be intreduced
                printf("- Sorry, none of your friends can help introduce you to %s.", connection_name);

            }
            printf("\n");


        } else if( strcmp(command, "AddFriendship") == 0){

            int already_friend = 0; // to store the value from add_connection()

            /*
             scan the user name and the connection name, then set temp_pointer_to_users to be the value of find_user(),
             set already_friend to be the value of add_connection()
             if already_friend value is 1, then print an error because they are already friends, else print the line that was scaned from the file
            */
            fscanf(input_actions, "%s %s", user_name, connection_name);
            temp_ponter_to_users = find_user(&head_of_users, user_name);
            already_friend = add_connection(&temp_ponter_to_users->connections, connection_name);

            if (already_friend == 1){
                printf("%s %s %s ExistingFriendshipError\n", command, user_name, connection_name);
            } else {

                temp_ponter_to_users = find_user(&head_of_users, connection_name);
                add_connection(&temp_ponter_to_users->connections, user_name);

                printf("%s %s %s\n", command, user_name, connection_name);
            }


        } else if( strcmp(command, "RemoveFriendship") == 0){

            int not_a_friend = 0; // to store the value returned from  remove_connection

            /*
             scan the user name and the connection name, then set temp_pointer_to_users to be the value of find_user(),
             set not_a_friend to be the value of remove_connection()
            */
            fscanf(input_actions, "%s %s", user_name, connection_name);
            temp_ponter_to_users = find_user(&head_of_users, user_name);
            not_a_friend = remove_connection(&temp_ponter_to_users->connections, connection_name);


            /*
             if not_a_friend is zero, remove the user from the user's connections and do the same for the other user.
             if the result of the second remove_connection friend is zero, then print the line that was scaned.
             if any of the conditions are false then print an error
            */
            if (not_a_friend == 0){
                temp_ponter_to_users = find_user(&head_of_users, connection_name);
                not_a_friend = remove_connection(&temp_ponter_to_users->connections, user_name);
                if (not_a_friend == 0) {
                    printf("%s %s %s\n", command, user_name, connection_name);
                } else {
                    printf("%s %s %s NoFriendshipError\n", command, user_name, connection_name);
                }

            } else {

                printf("%s %s %s NoFriendshipError\n", command, user_name, connection_name);
            }


        }

        if (feof(input_actions)) { // check if the file is empty, if it is empty then break from the loop
            break;
        }
        command[0] = '\0';      // reset command
        path = NULL;            // reset path
        visited_nodes = NULL;   // reset visited linked list
        stack_size = 0;         // reset the stack size
    }

    fclose(input_actions); // close file after reading everything in it

    return 0;
}


/*
  this function add users to the structure Vertices
    **head_of_users: the head of Vertices
    *user_name : the user we want to add
    *connection_name: the name of the user that we want to be connected to the added user

    this function returns nothing
 */
void add_user(Vertices **head_of_users, char *user_name, char *connection_name){

    Vertices *temp_ptr_to_users_head = *head_of_users;      // pointer to the head of users/ Vertices
    Vertices *temp_to_insert = malloc(sizeof(Vertices));    // pointer to insert

    // set temp_to_insert to be the values we want to inter to the list
    strcpy(temp_to_insert->user_name, user_name);
    temp_to_insert->next = NULL;
    temp_to_insert->prev = NULL;


    /*
     if head of users is empty, or the head's name is the same as user_name, or if the user_name is smaller,
     than the name stored in the head, then enter the if statement
    */
    if ( (*head_of_users == NULL) || ( strcmp( (*head_of_users)->user_name, user_name) == 0 ) || ( strcmp( (*head_of_users)->user_name, user_name) > 0 ) ) {

        /*
         if head of users is empty, then the temp_to_insert is set to be the head, and add connection_name to the connections of the head,
         else if head is the user we are looking for, then add connection_name to its connections
         else if user_name is less than the name in head, then insert temp_to_insert before the head
        */
        if (*head_of_users == NULL){
            *head_of_users = temp_to_insert;
            add_connection( &(*head_of_users)->connections, connection_name);

        } else if (strcmp( (*head_of_users)->user_name, user_name) == 0 ) {

            add_connection( &(*head_of_users)->connections, connection_name);

        } else if(( strcmp( (*head_of_users)->user_name, user_name) > 0 )){
            temp_to_insert->next = *head_of_users;
            (*head_of_users)->prev = temp_to_insert;
            *head_of_users = temp_to_insert;
        }


    } else {
        /*
         loop though until temp_ptr_to_users_head next i node is empty, or temp_ptr_to_users_head's next node is the same or smaller than  user_name,
        */
        while( (temp_ptr_to_users_head->next != NULL) && ( strcmp(temp_ptr_to_users_head->next->user_name, user_name) != 0) && ( strcmp( temp_ptr_to_users_head->user_name, user_name) < 0 )   ){
            temp_ptr_to_users_head = temp_ptr_to_users_head->next;
        }

        if (  temp_ptr_to_users_head != NULL){ // if temp_ptr_to_users_head is not empty then enter

            if(temp_ptr_to_users_head->next != NULL){ // the the next node is not empty then enter

                if ( strcmp(temp_ptr_to_users_head->next->user_name, user_name) == 0){ // if the user name is the same is the next node's name then add to connection_name to the connection of that user
                    add_connection(&temp_ptr_to_users_head->next->connections, connection_name);

                } else if( ( strcmp( temp_ptr_to_users_head->next->user_name, user_name) > 0 ) ){ // if next node's name is larger than user_name, then insert temp_to_insert before it

                    add_connection(&temp_to_insert->connections, connection_name);
                    temp_to_insert->prev = temp_ptr_to_users_head->prev;
                    temp_to_insert->next = temp_ptr_to_users_head;
                    temp_ptr_to_users_head->prev->next = temp_to_insert;


                }else { // if user_name is not smaller than the next node's name then insert it after

                    temp_to_insert->next = temp_ptr_to_users_head->next->next;
                    temp_ptr_to_users_head->next->prev = temp_to_insert;
                    temp_ptr_to_users_head->next = temp_to_insert;
                    add_connection(&temp_ptr_to_users_head->next->connections, connection_name);

                }

            } else { // if next node is empty

                if ( strcmp(temp_ptr_to_users_head->user_name, user_name) == 0){ // if the last node is the same as user_name, then add connection_name to its connecions
                    add_connection(&temp_ptr_to_users_head->connections, connection_name);

                }else if( ( strcmp( temp_ptr_to_users_head->user_name, user_name) > 0 ) ) { // if user_name is smaller then the last nodem then inset temp_to_insert befoe the last node

                    add_connection(&temp_to_insert->connections, connection_name);

                    temp_ptr_to_users_head->prev->next = temp_to_insert;
                    temp_to_insert->prev = temp_ptr_to_users_head->prev;
                    temp_ptr_to_users_head->prev = temp_to_insert;
                    temp_to_insert->next = temp_ptr_to_users_head;

                } else { // if the last node does not equal the last node, and the last node is smaller than user_name, then insert temp_to_insert to the end

                    temp_ptr_to_users_head->next = temp_to_insert;
                    temp_to_insert->prev = temp_ptr_to_users_head;
                    add_connection(&temp_ptr_to_users_head->next->connections, connection_name);
                }

            }


        }

    }

}

/*
  this function add users to the structure Connections
    **head_of_connections: the head of connecion of a specific user
    *connection_name: the name of the user that we want to be connected to that specific user

    this function returns ONE if the user is not added, or ZERO of the user is added
 */
int add_connection(Connections **head_of_connections, char *connection_name){

    Connections *temp_ptr_to_connection;                            // temporary pointer to Conections
    Connections *temp_to_inser = malloc(sizeof(Connections));       // the pointer that need to be inserted

    strcpy(temp_to_inser->user_name, connection_name);
    temp_to_inser->next = NULL;

    if (*head_of_connections == NULL){ // if head of connections is empty, then set temp_to_insert to be the new head
        *head_of_connections = temp_to_inser;

    } else { // if head of conection is not empty
        temp_ptr_to_connection = *head_of_connections; // set temp_ptr_to_connection to point to head of connection

        /*
          loop until temp_ptr_to_connection's next pointer is empty, or temp_ptr_to_connection name is the same as connection_name
         */
        while( (temp_ptr_to_connection->next != NULL) && (strcmp(temp_ptr_to_connection->user_name, connection_name) != 0 )) {
            temp_ptr_to_connection = temp_ptr_to_connection->next;
        }

        if (temp_ptr_to_connection->next != NULL){ // if temp_ptr_to_connection's next if not empty then enter

            if ( strcmp(temp_ptr_to_connection->user_name, connection_name) == 0){ // if the connection_name already exist then return 1
                return 1;
            }
            /*
              if connection_name is not in the list already, then add it to the middle of list
             */
            temp_to_inser->next = temp_ptr_to_connection->next->next;
            temp_ptr_to_connection->next = temp_to_inser;

        } else { // if the connection_name is not found then add it to the end of the list
            temp_ptr_to_connection->next = temp_to_inser;
        }
    }
    return 0;

}

/*
  this function remove users from the structure Connections
    **head_of_connections: the head of connecion of a specific user
    *connection_name: the name of the user that we want to be removed from that specific user connections

    this function returns ONE if the user is not removed, or ZERO of the user is removed
 */
int remove_connection(Connections **head_of_connections, char *connection_name){

    Connections *temp_ptr_to_connection;    // temporary pointer to connections
    Connections *temp_to_delete = NULL;     // temporary pointer to delete the found node

    if (*head_of_connections != NULL){ // if head is not empty then enter
        temp_ptr_to_connection = *head_of_connections; // set temp_ptr_to_connection to point to head

        if ((strcmp(temp_ptr_to_connection->user_name, connection_name) == 0)){ // if head is the node we are looking for, then set temp_to_delete to be temp_ptr_to_connection, and move temp_ptr_to_connection to be next
            temp_to_delete = temp_ptr_to_connection;
            temp_ptr_to_connection = temp_ptr_to_connection->next;

        } else { // if the head is not the node needed , then loop thugh elements while next node is not empty, and next's name is not what we are looking for

            while( (temp_ptr_to_connection->next != NULL) && (strcmp(temp_ptr_to_connection->next->user_name, connection_name) != 0) ){
                temp_ptr_to_connection = temp_ptr_to_connection->next;
            }

            if (temp_ptr_to_connection->next != NULL){ // if next is not empty

                if ( (strcmp(temp_ptr_to_connection->next->user_name, connection_name) == 0) ){ // if next name is what we are looking form then set temp_to_delete and connect the previous and next nodes with each others
                    temp_to_delete = temp_ptr_to_connection->next;
                    temp_ptr_to_connection->next = temp_ptr_to_connection->next->next;

                }
            }

        }

        if (temp_to_delete != NULL) { // if temp_to_delete is not empty then free it and return 0, else return 1
            free(temp_to_delete);
            return 0;
        } else {
            return 1;

        }

    }

}

/*
  this function add a node to the Stack
    **path: the head of the stack
    *size: the current size of the stack

    this function returns nothing
 */
void push(Stack **path, char *name, int *size){
    Stack *temp_to_insert = malloc(sizeof(Stack)); // a pointert to insert to the stack

    /*
      set the new pointer to hold the information needed
     */
    strcpy(temp_to_insert->user_name, name);
    temp_to_insert->next = *path;
    temp_to_insert->prev = NULL;

    if (*path == NULL){ // if the stack is empty then update its head and increase the size of the stack
        *path = temp_to_insert;
        (*size)++;
    } else { // if head is not empty, then add temp_to_insert to be the new head, and increase the stack size
        (*path)->prev = temp_to_insert;
        temp_to_insert->next = *path;
        *path = temp_to_insert;
        (*size)++;
    }

}

/*
  this function remove a node from the Stack
    **path: the head of the stack
    *size: the current size of the stack

    this function returns nothing
 */
void pop(Stack **path, int *size){

    Stack *temp_to_delete; // a temporary pointer to hold the node we need to delete

    if (*size > 0 && *path != NULL) { // if the stack size is bigger than zero and the stack is not empty, then set temp_to_delete to be the head, then update its pointers
        temp_to_delete = *path;
        if ((*path)->next != NULL) {
            (*path)->next->prev = NULL;
        }

        *path = (*path)->next; // update the top of the stack
        free(temp_to_delete);  // free the node from the stack

        (*size)--; // decrease the size of the stack
    }

}

/*
  this function add users to the structure Connections
    **visited: the head of visited linked list
    *name: the name of the node that we want to add to the liked list Visited

    this function returns nothing
 */
void add_to_visited(Visited **visited, char *name){

    Visited *temp_pointer;                              // temporary pointer to Visited head
    Visited *temp_to_insert = malloc(sizeof(Visited));  // the node we need to insert

    strcpy(temp_to_insert->name, name);
    temp_to_insert->next = NULL;

    if (*visited == NULL){ // if the head is empty, then set head to be temp_to_insert
        *visited = temp_to_insert;
    } else { // if head is not empty, set temp_pointer to point to Visite head, and loop until the next node is empty
        temp_pointer = *visited;

        while((temp_pointer->next != NULL)){

            if ((strcmp(temp_pointer->name, name) == 0)) { // if the name already exist in the Visited list, then return and end the function
                return;
            }

            temp_pointer = temp_pointer->next;
        }

        if ((strcmp(temp_pointer->name, name) == 0)) { // if the current name equals name, then stop the function and return
            return;

        } else if (temp_pointer->next == NULL){ // if next node is empty, then add temp_to_insert to the end of the list
            temp_pointer->next = temp_to_insert;

        } else {
            return;
        }

    }

}

/*
  this function is to find if the user is in the list of not
    **visited: the head of visited linked list
    *name: the name of the node that we want to find in the liked list Visited

    this function returns ONE if the node is found, and ZERO if the node is not found
 */
int is_visited(Visited **visited, char *name) {

    Visited *temp_pointer; // a temporary pointer to Visited head
    int is_visited = 0;    // a value to return

    if (*visited != NULL){  // if head of Visited is not empty, set temp_pointer to be Visited head
        temp_pointer = *visited;

        while( temp_pointer != NULL){ // loop until temp_node is empty

            if ( strcmp(temp_pointer->name, name) == 0){ // if temp_node and name are equal, then update is_visited to be 1
                is_visited = 1;
            }
            temp_pointer = temp_pointer->next;
        }

    }

    return is_visited; // return is_visited
}

/*
  this function is to find a user in the structure Vertices
    **users: the head of Vertices
    *user_to_find: the name of the node that we want to find in the liked list Visited

    this function returns a pointer to the node if found, if node is not found it will return NULL
 */
Vertices *find_user(Vertices **users, char *user_to_find){

    Vertices *temp_pointer = NULL;  // temporary pointer to Visited head

    if (*users != NULL) {   // if head of Visited is not empty, then set temp_pointer to be *users
        temp_pointer = *users;

        while( temp_pointer != NULL ){ // loop until temp_pointer is empty, or the current node and user_to_find have the same value
            if (strcmp(temp_pointer->user_name, user_to_find) == 0) {
                break;
            }
            temp_pointer = temp_pointer->next;
        }
    }
    return temp_pointer; // return temp_pointer which hold a pointer to the user we needed
}

/*
  this function is to find a user in the structure Vertices
    **users: the pointer to a specific node on the structure Vertices
    *start: the name of the starting node or user
    *target: the targeted user that we want to connect with
    **path: a stack that hold the path from *start to *target if there is any
    **visited: a structure that keep track of what we have visited
    *size: the size of the stack

    this function returns a pointer to the stack if there is connection between the start user in the target user,
    if there is no path between start and target, then it returns NULL
 */
Stack *find_path(Vertices **users, char *start, char *target, Stack **path, Visited **visited, int *size) {

    Vertices * temp_pointer_to_users = find_user(&(*users), start);  // temporary pointer that points to the node that has the start name
    Stack *right_path = NULL;                                        // a pointer to hold the path from start to target
    Connections *temp_pointer_to_connection = NULL;                  // tmporary pointer to Connections
    int is_it_visited = 0;


    /*
      add the start value to Visited and then push it to the stack path
      if temp_pointer_to_users is not empty and its name equals target then return path
     */
    add_to_visited(&(*visited), start);
    push(&(*path), start, size);
    if (temp_pointer_to_users != NULL){
        if ( strcmp(temp_pointer_to_users->user_name, target) == 0 ){
            (*size)--;
            return *path;

        }
    }

    /*
      set right_path to be empty, and if temp_pointer_to_users is not empty then assign its connections to temp_pointer_to_connection
     */
    right_path = NULL;
    if (temp_pointer_to_users != NULL){
        temp_pointer_to_connection = temp_pointer_to_users->connections;
    }


    if ( temp_pointer_to_connection != NULL ) { // if connections are not empty

        /*
          loop until temp_pointer_to_connection is empty, and assign the value from is_visited() to is_it_visited,
          if is_it_sivited is zero then set right_path to be the value of find_path()

         */
        while(temp_pointer_to_connection != NULL) {

            is_it_visited = is_visited( &(*visited) , temp_pointer_to_connection->user_name);
            if (is_it_visited != 1){
                right_path = find_path(&(*users), temp_pointer_to_connection->user_name, target, &(*path), &(*visited), size);

                if ( (right_path != NULL) ) { // if right_path is not empty, then return it
                    return right_path;
                }

            }

            /*
              if size is more than or equals to 4, then remove the last node that was intered to Visited.
              then update temp_pointer_to_connection to be its next
             */
            if (*size >= 4){
                remove_from_visited(&(*visited), (*path)->user_name);
                break;
            }
            temp_pointer_to_connection = temp_pointer_to_connection->next;

        }

    }


    pop(&(*path), size); // remove the top of the stack
    return NULL;


}

/*
  this function remove users from the structure Visited
    **visited: the head of visited linked list
    *name: the name of the node that we want to remove from the liked list Visited

    this function returns nothing
 */
void remove_from_visited(Visited **visited, char *name){

    Visited *temp_pointer;                      // temporary pointer to
    Visited *temp_pointer_to_delete = NULL;     // temporary pointer to hold the node we want to delete

    if (*visited != NULL){ // if visited is not empty, then enter
        temp_pointer = *visited; // set temp_pointer to be the head of visited

        /*
          if the head node equals the name we are trying to find , then set temp_pointer_to_delete to be that node, and update the head
         */
        if ((strcmp(temp_pointer->name, name) == 0)){
            temp_pointer_to_delete = temp_pointer;
            temp_pointer = temp_pointer->next;

        } else {
            /*
              loop until the next node is empty, or the name of the next time is what we are looking for
             */
            while( (temp_pointer->next != NULL) && (strcmp(temp_pointer->next->name, name) != 0) ){
                temp_pointer = temp_pointer->next;
            }

            /*
              if next node is not empty, then set temp_pointer_to_delete to be next node, and update next node
             */
            if (temp_pointer->next != NULL){

                if ( (strcmp(temp_pointer->next->name, name) == 0) ){
                    temp_pointer_to_delete = temp_pointer->next;
                    temp_pointer->next = temp_pointer->next->next;

                }
            }

        }
        free(temp_pointer_to_delete); // free the node that we found

    }

}
