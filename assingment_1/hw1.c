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
  A structure that have all workers who are avaliable and it will be treated as a queue
*/
typedef struct Workers {
    char workers_name[30];
    struct Workers *next;
} Workers;

/*
  a structure that stores the details of costumers orders
*/
typedef struct Costumer_orders{
    int order_time;
    char costumers_name[30];
    int books_ordered;
    int electronics_ordered;
    struct Costumer_orders *next;
} Costumer_orders;

/*
  a structure that will have the information of the costumers and the worker whose taking care of them.
*/
typedef struct worker_assignment{
    // char command[25];
    int order_time;
    char worker_name[30];
    char first_costumers_name[25];
    int order_completion_time;
    int done;
    struct worker_assignment *next;
} Worker_assignment;

/*
  a structure that stores cosumers' who ordered from only one category information so.
  Then it will be passed to functions as a parameter to use the information in it
*/
typedef struct  {
    int order_time;
    char costumers_name[10][30];
    int books_ordered;
    int electronics_ordered;
} Multi_Orders;


/*
  this function is to add workers who finished their work to the end of the Queue Workers.

  this function does not returne anything
 */
void enqueue( char name[30]);

/*
  This function is to delete a worker from the front of the queue Workers

  returnes the workers name
 */
char *dequeue();


/*
  This function is to take information from the file and add them to the limked list Costumers_orders

  name is costumer's name
  order_time is the time when order is placed
  books is the number of books ordered
  Electronics is the number of electronics ordered

  returns nothing
 */
void add_order( char name[30], int order_time, int books, int electronics);


/*
  This function is to assign a worker from the Queue Workers to work on an order requested

  does not returne anything
 */
void assign_worker_to_one_costumer();


/*
  This function is to assign a worker from the Queue Workers to work on many orders that has either books or electronics

  boundles is of type struct Multi_Orders that hold information of more than one order

  does not returne anything
 */
void assign_worker_to_many_costumer(Multi_Orders boundles);


/*
  This function is to determain if the order is complete or not. Then, it will delete the order that is complete from the linked-list
  and add the worker to the Queue Workers which means he is availabel

  *current_time is a pointer to the current time when the program is running so we can decide of the order is finished or not

  does not returne anything
 */
void order_completed_for_one( int *current_time);


/*
  This function is to determain if the order is complete or not. Then, it will delete the order that is complete from the linked-list
  and add the worker to the Queue Workers which means he is availabel (used at the end to empty Worker_assignment linked list)

  *current_time is a pointer to the current time when the program is running so we can decide of the order is finished or not

  does not returne anything
 */
void order_completed_for_the_end(int *current_time);


/*
  This function is to print the names of the workers who are assigned to orders and are working

  current_time is the last current time was scaned from the file

  does not returne anything
 */
void workers_assigned_list( int current_time);


/*
  This function is to print the names of the workers who are availabe

  does not returne anything
 */
void workers_available_list();

/*
  This function is to calculate the longiest that a worker took to finish an order

  returnes the number of minutes spent on the longest order
 */
int max_fulfillment_time(int last_max_fulfillment_time);


/*
  Global pointers to the beginning to each liked list
*/
Workers *front = NULL;                                      // pointer to the front of Workers list
Costumer_orders *head_pointer_to_orders = NULL;             // pointer to the beginning Costumer_orders list
Worker_assignment *head_pointer_to_workers_assigned = NULL; //pointer to the beginning of Worker_assignment list

int main(int argc, char* argv[]){
    /*
      description of declared variables
     */

     /*
       an array that has all the Workers names to add them to the Queue Workers
     */
    char workers_names[5][30] = {"Alice",
                                 "Bob",
                                 "Carol",
                                 "David",
                                 "Emily"};
    /*
       two variables of type Multi_Orders to handle more than one order that have either books or electronics
    */
    Multi_Orders books_boundles;
    Multi_Orders electronics_boundles;



    /*
       these variables are to hold the information from the file so they can be used in other functions
    */
    char command[30];      // to read the command at the beginning of each like from the input file
    int time_order_placed;      // to store the time from the input file
    char name[30];         // to store the name of the costumer if needed
    int books = 0;         // to store the number of books if needed
    int electronics = 0;   // to store the number of books if needed
    int current_time = 0; // the time for the whole program

    /*
       a variavle to hold the number of minutes that the longest order take to complete
    */
    int maxFulfillmentTime = 0;


    /*
       pointer to open the input file
    */
    FILE *input_file = fopen( argv[1], "r" );  // variable to point to the file

    /*
       this for loop is to insert the workers into the queue
    */
    for (int i = 0; i < 5; i++) {
        enqueue( workers_names[i]);
    }

    /*
       check if the file opens
       if yes we go to else
    */
    if (input_file == NULL) {
        printf("ERROR: file did not open currectly\n");
        perror(argv[1]);

    } else {

      /*
        initiate the beginning values for the book boundle
      */
        books_boundles.order_time = 0;
        books_boundles.books_ordered = 0;
        books_boundles.electronics_ordered = 0;

        /*
          initiate the beginning values for the electronics boundle
        */
        electronics_boundles.order_time = 0;
        electronics_boundles.books_ordered = 0;
        electronics_boundles.electronics_ordered = 0;

        // set all the strings to \0 to initiate them for using
        for (int i = 0; i < 10; i++) {
            books_boundles.costumers_name[i][0] = '\0';
            electronics_boundles.costumers_name[i][0] = '\0';
        }

        /*
          to keep track of the number of boundled orders we have
        */
        int electronics_orders_counter = 0;
        int books_orders_counter = 0;

        while (!feof(input_file)) {

            fscanf(input_file, "%s", command);

            /*
              we scan the first string in the file to know what is the appropriate action to take after that
            */
            if (strcmp(command ,"CustomerOrder") == 0) {
                fscanf(input_file, "%d %s %d %d", &current_time, name, &books, &electronics);
                time_order_placed = current_time;

                /*
                  if books or electronics are zero in the order we enter this if-statement to add the order to the boundle

                  it has more than one coundition to make sure when to add them and when to assignm them to a worker
                */
                if ((books == 0) || (electronics == 0)) {
                    if (electronics != 0) {

                        if (electronics_boundles.electronics_ordered <= 10) {
                            electronics_boundles.order_time = time_order_placed; // assign time here so we can use it in the next if-statement

                            if ((electronics_boundles.order_time + 5) > current_time) {

                                if (electronics_orders_counter <= 10) {
                                  /*
                                    update MaxFulfillmentTime and check if there is any order completed
                                  */
                                    maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
                                    order_completed_for_one(&current_time);

                                    /*
                                      once it pass all the presious conditions we can know add it to the boundles of electronics
                                      Then, print to the screen that we added it
                                    */
                                    electronics_boundles.electronics_ordered = electronics;
                                    strcpy(electronics_boundles.costumers_name[electronics_orders_counter], name);
                                    electronics_boundles.books_ordered = books;
                                    electronics_boundles.electronics_ordered = electronics;

                                    printf("CustomerOrder ");
                                    printf("%d ", electronics_boundles.order_time);
                                    printf("%s ", electronics_boundles.costumers_name[electronics_orders_counter]);
                                    printf("%d ", electronics_boundles.books_ordered);
                                    printf("%d\n", electronics_boundles.electronics_ordered);


                                    electronics_orders_counter++;
                                } else {
                                  /*
                                  make sure the program assign a worker to all orders in our Costumers_orders linnke list.
                                  Then, the program reinitiate all the value of the boundle to use again later
                                  Then, the program checks if there is any orders compeleted
                                  */
                                    assign_worker_to_many_costumer(electronics_boundles);

                                    for (int i = 0; i < 10; i++) {
                                        electronics_boundles.costumers_name[i][0] = '\0';
                                    }
                                    electronics_boundles.books_ordered = 0;
                                    electronics_boundles.electronics_ordered = 0;
                                    electronics_orders_counter = 0;

                                    order_completed_for_one(&current_time);
                                }

                            } else {
                              /*
                              Update maxFulfillmentTime time first.
                              Then, the program checks if there is any orders compeleted.
                              Then, make sure the program assign a worker to all orders in the boundle .
                              Then, the program reinitiate all the value of the boundle to use again later
                              finaly check if there are orders to assign workers to
                              */

                              electronics_boundles.order_time = time_order_placed;
                              maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
                              order_completed_for_one(&current_time);

                              assign_worker_to_many_costumer(electronics_boundles);

                              for (int i = 0; i < 10; i++) {
                                  electronics_boundles.costumers_name[i][0] = '\0';
                              }
                              electronics_boundles.books_ordered = 0;
                              electronics_boundles.electronics_ordered = 0;
                              electronics_orders_counter = 0;

                              assign_worker_to_one_costumer();

                            }

                        } else {
                          /*
                          Update maxFulfillmentTime time first.
                          Then, the program checks if there is any orders compeleted.
                          Then, make sure the program assign a worker to all orders in the boundle .
                          Then, the program reinitiate all the value of the boundle to use again later
                          */
                          maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
                          order_completed_for_one(&current_time);

                          assign_worker_to_many_costumer(electronics_boundles);

                          for (int i = 0; i < 10; i++) {
                              electronics_boundles.costumers_name[i][0] = '\0';
                          }
                          electronics_boundles.books_ordered = 0;
                          electronics_boundles.electronics_ordered = 0;
                          electronics_orders_counter = 0;

                        }

                    } else if (books != 0) {

                        if (books_boundles.electronics_ordered <= 10)  {
                            books_boundles.order_time = time_order_placed;  // assign time here so we can use it in the next if-statement

                            if ((books_boundles.order_time + 5) > current_time) {
                                (current_time + 5) >books_boundles.order_time;

                                if (books_orders_counter <= 10) {
                                  /*
                                    update MaxFulfillmentTime and check if there is any order completed
                                  */
                                  maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
                                  order_completed_for_one(&current_time);

                                  /*
                                    once it pass all the presious conditions we can know add it to the boundles of electronics
                                    Then, print to the screen that we added it
                                  */
                                  books_boundles.electronics_ordered = electronics;
                                  strcpy(books_boundles.costumers_name[books_orders_counter], name);
                                  books_boundles.books_ordered = books;
                                  books_boundles.electronics_ordered = electronics;

                                  printf("CustomerOrder ");
                                  printf("%d ", books_boundles.order_time);
                                  printf("%s ", books_boundles.costumers_name[books_orders_counter]);
                                  printf("%d ", books_boundles.books_ordered);
                                  printf("%d\n", books_boundles.electronics_ordered);

                                  books_orders_counter++;

                                } else {

                                  /*
                                  make sure the program assign a worker to all orders in our Costumers_orders linnke list.
                                  Then, the program reinitiate all the value of the boundle to use again later
                                  Then, the program checks if there is any orders compeleted
                                  */
                                  assign_worker_to_many_costumer(books_boundles);

                                  if (books_orders_counter != 0) {

                                  }
                                  for (int i = 0; i < 10; i++) {
                                      books_boundles.costumers_name[i][0] = '\0';
                                  }
                                  books_boundles.books_ordered = 0;
                                  books_boundles.electronics_ordered = 0;
                                  books_orders_counter = 0;
                                  books_orders_counter = 0;

                                  order_completed_for_one(&current_time);
                                }

                            } else {

                              /*
                              Update maxFulfillmentTime time first.
                              Then, the program checks if there is any orders compeleted.
                              Then, make sure the program assign a worker to all orders in the boundle .
                              Then, the program reinitiate all the value of the boundle to use again later
                              finaly check if there are orders to assign workers to
                              */

                              books_boundles.order_time = time_order_placed;
                              maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
                              order_completed_for_one(&current_time);


                              assign_worker_to_many_costumer(books_boundles);

                              for (int i = 0; i < 10; i++) {
                                  books_boundles.costumers_name[i][0] = '\0';
                              }
                              books_boundles.books_ordered = 0;
                              books_boundles.electronics_ordered = 0;
                              books_orders_counter = 0;
                              books_orders_counter = 0;

                              assign_worker_to_one_costumer();

                          }

                        } else {
                          /*
                          Update maxFulfillmentTime time first.
                          Then, the program checks if there is any orders compeleted.
                          Then, make sure the program assign a worker to all orders in the boundle .
                          Then, the program reinitiate all the value of the boundle to use again later
                          */
                          maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
                          order_completed_for_one(&current_time);

                          assign_worker_to_many_costumer(books_boundles);
                          if (books_orders_counter != 0) {

                          }
                          for (int i = 0; i < 10; i++) {
                              books_boundles.costumers_name[i][0] = '\0';
                          }
                          books_boundles.books_ordered = 0;
                          books_boundles.electronics_ordered = 0;
                          books_orders_counter = 0;
                          books_orders_counter = 0;

                        }

                    }

                } else {
                  /*
                  Update maxFulfillmentTime time first
                  Check if there are any orders compeleted
                  Add the order
                  If the Boundle DOES NOT Equal zero then assign a worker to it
                  Then, reinitiate the members of the Boundle to reuse it again
                  Then, assign a worker to the order we add at the beginning
                  */

                  maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
                  order_completed_for_one(&current_time);
                  add_order( name, time_order_placed, books, electronics);

                  if (books_orders_counter != 0) {
                      books_boundles.order_time = time_order_placed;
                      assign_worker_to_many_costumer(books_boundles);

                      if (books_orders_counter != 0) {


                      }
                      for (int i = 0; i < 10; i++) {
                          books_boundles.costumers_name[i][0] = '\0';
                      }
                      books_boundles.books_ordered = 0;
                      books_boundles.electronics_ordered = 0;
                      books_orders_counter = 0;
                      books_boundles.order_time = 0;


                    }

                  if (electronics_orders_counter != 0) {
                      electronics_boundles.order_time = time_order_placed;
                      assign_worker_to_many_costumer(electronics_boundles);
                      electronics_orders_counter = 0;
                      if (electronics_orders_counter != 0) {

                      }
                      for (int i = 0; i < 10; i++) {
                          electronics_boundles.costumers_name[i][0] = '\0';
                      }
                      electronics_boundles.books_ordered = 0;
                      electronics_boundles.electronics_ordered = 0;
                      electronics_orders_counter = 0;
                      electronics_boundles.order_time = 0;

                  }

                  assign_worker_to_one_costumer();

                }

            } else if (strcmp(command ,"PrintAvailableWorkerList") == 0) {
              /*
              print availabe workers
              */
              fscanf(input_file, "%d ", &current_time);
              workers_available_list();

            } else if ( strcmp(command ,"PrintWorkerAssignmentList") == 0) {
              /*
              print availabe workers
              */
              fscanf(input_file, "%d ", &current_time);
              workers_assigned_list(current_time);

            } else if ( strcmp(command ,"PrintMaxFulfillmentTime") == 0) {
              /*
              Check if there are orders to assign a worker to.
              Print maxFulfillmentTime
              check if boundles empty, if one of them is not then we have to assign a worker to it.

              */
              assign_worker_to_one_costumer();
              maxFulfillmentTime = max_fulfillment_time(maxFulfillmentTime);
              printf("MaxFulfillmentTime %d\n", maxFulfillmentTime);

              if (electronics_orders_counter != 0) {
                  assign_worker_to_many_costumer(electronics_boundles);
                  electronics_orders_counter = 0;
                  if (electronics_orders_counter != 0) {
                      for (int i = 0; i < 10; i++) {
                          electronics_boundles.costumers_name[i][0] = '\0';
                      }
                  }
                  electronics_orders_counter = 0;
              }

              if (books_orders_counter != 0) {
                  assign_worker_to_many_costumer(books_boundles);

                  if (books_orders_counter != 0) {
                      for (int i = 0; i < 10; i++) {
                          books_boundles.costumers_name[i][0] = '\0';
                      }

                  }
                  books_orders_counter = 0;

              }

            }

        }





        /*
        This is to make sure that the last assigned orders to workers are going to ge completed before the program terminates
        */
        while ( (head_pointer_to_workers_assigned != NULL) ) {

            order_completed_for_the_end(&current_time);
            head_pointer_to_workers_assigned = head_pointer_to_workers_assigned->next;
        }

    }


    return 0;
}




/*
  this function is to add workers who finished their work to the end of the Queue Workers.

  this function does not returne anything
 */
void enqueue(char name[25]){
  /*
    to temporaraly pointer to use in two different while loops for checking
   */
  Workers *temperary_pointer_to_front;
  Workers *temp_pointer_to_front_second;

  /*
    initiate the node that we want to insert
   */
  Workers *new_node_to_insert = malloc(sizeof(Workers));
  strcpy(new_node_to_insert->workers_name, name);
  new_node_to_insert->next = NULL;

  if (front == NULL) {
      front = new_node_to_insert;

  } else {

    /*
      to check if the name is already there
      if it is there then sop the function
     */
    temp_pointer_to_front_second = front;
    while(temp_pointer_to_front_second != NULL){
        if( strcmp(temp_pointer_to_front_second->workers_name, name) == 0){
            return;
        }

        temp_pointer_to_front_second = temp_pointer_to_front_second->next;
    }

    /*
      to check if the name not already in the queue
      Then add it
     */
    temperary_pointer_to_front = front;

    while (temperary_pointer_to_front->next != NULL) {
        temperary_pointer_to_front = temperary_pointer_to_front->next;
    }
    temperary_pointer_to_front->next = new_node_to_insert;
  }
}


/*
  This function is to delete a worker from the front of the queue Workers

  returnes the workers name
 */
char *dequeue() {
  /*
    declear an array to hold the name of the worker  so we can returne it at the end
   */

  char *temp_name_to_returne = malloc(30*sizeof(30));
  Workers *temp_pointer;

  /*
    take the first name in the queue if it's not empty and returne it
   */
  if (front != NULL) {
      strcpy(temp_name_to_returne, front->workers_name);
      temp_pointer = front;
      front = front->next;
      free(temp_pointer);
  }
  return temp_name_to_returne;
}


/*
  This function is to take information from the file and add them to the limked list Costumers_orders

  name is costumer's name
  order_time is the time when order is placed
  books is the number of books ordered
  Electronics is the number of electronics ordered

  returns nothing
 */
void add_order( char name[30], int order_time, int books, int electronics){
  /*
    a a temporarly pointer to the list COstumer_orders
   */
  Costumer_orders *temp_pointer_to_list_head;

  /*
    ideclear a node and malloc it to use to hold information and then add it
   */
  Costumer_orders *temp_to_hold_info = malloc(sizeof(Costumer_orders));

  /*
    assign the new order's values to the new node
   */
  strcpy(temp_to_hold_info->costumers_name , name);
  temp_to_hold_info->order_time = order_time;
  temp_to_hold_info->books_ordered = books;
  temp_to_hold_info->electronics_ordered = electronics;
  temp_to_hold_info->next = NULL;

  /*
    if the beginning of Costumers_orders is not NULL then print
   */
  if (head_pointer_to_orders == NULL) {
      head_pointer_to_orders = temp_to_hold_info;

      printf("CustomerOrder ");
      printf("%d ", temp_to_hold_info->order_time);
      printf("%s ", temp_to_hold_info->costumers_name);
      printf("%d ", temp_to_hold_info->books_ordered);
      printf("%d\n", temp_to_hold_info->electronics_ordered);


  } else {
    /*
      if the list is not empty then go to the last node in it
      then add the order to the next of the last node
      then print that a costumer has ordered something
     */
      temp_pointer_to_list_head = head_pointer_to_orders;
      while (temp_pointer_to_list_head->next != NULL) {
          temp_pointer_to_list_head = temp_pointer_to_list_head->next;
      }
      temp_pointer_to_list_head->next = temp_to_hold_info;

      printf("CustomerOrder ");
      printf("%d ", temp_to_hold_info->order_time);
      printf("%s ", temp_to_hold_info->costumers_name);
      printf("%d ", temp_to_hold_info->books_ordered);
      printf("%d\n", temp_to_hold_info->electronics_ordered);
  }

}


/*
  This function is to assign a worker from the Queue Workers to work on an order requested

  does not returne anything
 */
void assign_worker_to_one_costumer() {
    /*
      this pointer is to handle Worker_assignment list if it is not NULL
     */
    Worker_assignment *pointer_temp_to_worker_assignment_head;

    /*
      this pointer is to store information in before adding it to the list
     */
    Worker_assignment *temp_to_hold_info = malloc(sizeof(Worker_assignment));;


    /*
      this pointer is to delete the node from Costumer_orders after copying the information from them.
     */
    Costumer_orders *temp_pointer_to_orders_head;



    if (head_pointer_to_orders != NULL) {
      if (front == NULL) {
        printf("There are no workers available at this time\n");
        return;
      }

      if (head_pointer_to_workers_assigned == NULL) {
      /*
        if the list of COstumer_orders not NULL, and the list of Worker_assignment is NULL
        then compy information to it and calculate the time of ecpected completion
       */
        temp_to_hold_info->order_time = head_pointer_to_orders->order_time;
        temp_to_hold_info->order_completion_time = (head_pointer_to_orders->order_time) + 5;


        if (head_pointer_to_orders->books_ordered != 0) {
            temp_to_hold_info->order_completion_time += head_pointer_to_orders->books_ordered ;
        }

        if (head_pointer_to_orders->electronics_ordered != 0) {
            temp_to_hold_info->order_completion_time += head_pointer_to_orders->electronics_ordered ;
        }

        if ( (head_pointer_to_orders->electronics_ordered != 0) && (head_pointer_to_orders->books_ordered != 0) ) {

            temp_to_hold_info->order_completion_time += 5;
        }

        /*
          assign worker and copy more information
          ->done = 1 means the order is not completed temperary_pointer_to_front
          Then, print that the order is assigned to a worker
         */
        strcpy( temp_to_hold_info->worker_name, dequeue());
        strcpy( temp_to_hold_info->first_costumers_name, head_pointer_to_orders->costumers_name);
        temp_to_hold_info->done = 1;
        temp_to_hold_info->next = NULL;

        printf("WorkerAssignment ");
        printf("%d ", temp_to_hold_info->order_time);
        printf("%s ", temp_to_hold_info->worker_name);
        printf("%s\n", temp_to_hold_info->first_costumers_name);

        /*
          add the new node to Worker_assignment list
          Then delete the node with the same information from Costumer_orders list
         */
        head_pointer_to_workers_assigned = temp_to_hold_info;
        head_pointer_to_workers_assigned->next = NULL;

        temp_pointer_to_orders_head = head_pointer_to_orders;
        head_pointer_to_orders = head_pointer_to_orders->next;
        free(temp_pointer_to_orders_head);

    } else {
      /*
        if the list of COstumer_orders NOT NULL, and the list of Worker_assignment is also NOT NULL
        then go to the last node in Worker_assignment list
        then copy information to it and calculate the time of ecpected completion
       */
        pointer_temp_to_worker_assignment_head = head_pointer_to_workers_assigned;
        while (pointer_temp_to_worker_assignment_head->next != NULL) {
            pointer_temp_to_worker_assignment_head = pointer_temp_to_worker_assignment_head->next;
        }

        /*
          if the list of COstumer_orders not NULL, and the list of Worker_assignment is NULL
          then compy information to it and calculate the time of ecpected completion
         */
        temp_to_hold_info->order_time = head_pointer_to_orders->order_time;
        temp_to_hold_info->order_time = head_pointer_to_orders->order_time;
        temp_to_hold_info->order_completion_time = (head_pointer_to_orders->order_time) +5;

        if (head_pointer_to_orders->books_ordered != 0) {
            temp_to_hold_info->order_completion_time += head_pointer_to_orders->books_ordered ;
        }

        if (head_pointer_to_orders->electronics_ordered != 0) {
            temp_to_hold_info->order_completion_time += head_pointer_to_orders->electronics_ordered ;
        }

        if ( (head_pointer_to_orders->electronics_ordered != 0) && (head_pointer_to_orders->books_ordered != 0) ) {

            temp_to_hold_info->order_completion_time += 5;
        }

        /*
          assign worker and copy more information
          ->done = 1 means the order is not completed temperary_pointer_to_front
          Then, print that the order is assigned to a worker
         */
        strcpy( temp_to_hold_info->worker_name, dequeue());
        strcpy( temp_to_hold_info->first_costumers_name, head_pointer_to_orders->costumers_name);
        temp_to_hold_info->done = 1;
        temp_to_hold_info->next = NULL;

        printf("WorkerAssignment ");
        printf("%d ", temp_to_hold_info->order_time);
        printf("%s ", temp_to_hold_info->worker_name);
        printf("%s\n", temp_to_hold_info->first_costumers_name);

        /*
          add the new node to Worker_assignment list
          Then delete the node with the same information from Costumer_orders list
         */
        pointer_temp_to_worker_assignment_head->next = temp_to_hold_info;
        temp_pointer_to_orders_head = head_pointer_to_orders;
        head_pointer_to_orders = head_pointer_to_orders->next;
        free(temp_pointer_to_orders_head);

    }


    }



}


/*
  This function is to assign a worker from the Queue Workers to work on many orders that has either books or electronics

  boundles is of type struct Multi_Orders that hold information of more than one order

  does not returne anything
 */
void assign_worker_to_many_costumer(Multi_Orders boundles){

  /*
    this pointer is to  Worker_assignment list
   */
  Worker_assignment *pointer_temp_to_worker_assignment_head;

  /*
    this node of type Worker_assignment list to add to list
   */
  Worker_assignment *temp_to_add; // to store information then add it to the linked list

  /*
    These are variables to hold information that are needed in every single order we assignm
    counter to how many orders in the boundle
     workers_name stores the workers name to asssign to all ccostumers
     completion_time to hold completion time that we need to assign to all orders
   */
  int counter = 0;
  char workers_name[30];
  int completion_time = 0;

  if (front == NULL) {
    printf("There are no workers available at this time\n");
    return;
  }


  if (head_pointer_to_workers_assigned == NULL) {
    /*
    is Worker_assignment is empty then we add to the beginning of it
    then calculate completion tim e
    then copy workers name to the variable
   */
    pointer_temp_to_worker_assignment_head = head_pointer_to_workers_assigned;
    completion_time += 10;
    completion_time += boundles.books_ordered;
    completion_time += boundles.electronics_ordered;
    strcpy( workers_name, dequeue());
    /*
      as long as the costumers_name array is not empty repeat this action
    */
    while ((boundles.costumers_name[counter][0] != '\0') && (counter < 10) ) {
      /*
        malloc the node each time because the program should add more than one node
      */
      temp_to_add = malloc(sizeof(Worker_assignment));

      /*
        copy information from the struct to the node temp_to_add
        then print time and workers name just once alont with  "WorkerAssignment "
      */
      temp_to_add->order_time = boundles.order_time;
      strcpy( temp_to_add->first_costumers_name, boundles.costumers_name[counter]);
      temp_to_add->order_completion_time = completion_time + boundles.order_time;
      strcpy( temp_to_add->worker_name, workers_name);

      if (counter == 0){
          printf("WorkerAssignment ");
          printf("%d ", temp_to_add->order_time);
          printf("%s ", temp_to_add->worker_name);

      }

      /*
        print costumers' name and add them to the next element of the linked list Worker_assignment
        ->done = 1 implies that the order is not completed yet
        then increase conter to go to the next costumer's name in the boundle
      */
      printf("%s ", temp_to_add->first_costumers_name);
      temp_to_add->done = 1;
      temp_to_add->next = NULL;

      pointer_temp_to_worker_assignment_head = temp_to_add;
      pointer_temp_to_worker_assignment_head = pointer_temp_to_worker_assignment_head->next;
      counter++;
    }
    printf("\n");
    /*
      reset all informat
      ion in boundle
    */
    boundles.order_time = 0;
    for (int i = 0; i < 10; i++) {
        boundles.costumers_name[i][0] = '\0';
    }
    boundles.books_ordered = 0;
    boundles.electronics_ordered = 0;

  } else {
    /*
    is Worker_assignment is NOT empty then we go  to the end of it
   */

    pointer_temp_to_worker_assignment_head = head_pointer_to_workers_assigned;
    while (pointer_temp_to_worker_assignment_head->next != NULL) {
        pointer_temp_to_worker_assignment_head = pointer_temp_to_worker_assignment_head->next;
    }

    /*
    then calculate completion time
    then copy workers name to the variable
    */
    completion_time += 10;
    completion_time += boundles.books_ordered;
    completion_time += boundles.electronics_ordered;
    strcpy( workers_name, dequeue());

    while ((boundles.costumers_name[counter][0] != '\0') && (counter < 10) ) {

      /*
        malloc the node each time because the program should add more than one node
      */
      temp_to_add = malloc(sizeof(Worker_assignment));

      /*
        copy information from the struct to the node temp_to_add
        then print time and workers name just once alont with  "WorkerAssignment "
      */
      temp_to_add->order_time = boundles.order_time;
      strcpy( temp_to_add->first_costumers_name, boundles.costumers_name[counter]);
      temp_to_add->order_completion_time = completion_time + boundles.order_time;
      strcpy( temp_to_add->worker_name, workers_name);

      if (counter == 0){
          printf("WorkerAssignment ");
          printf("%d ", temp_to_add->order_time);
          printf("%s ", temp_to_add->worker_name);

      }

      /*
        print costumers' name and add them to the next element of the linked list Worker_assignment
        ->done = 1 implies that the order is not completed yet
        then increase conter to go to the next costumer's name in the boundle
      */
      printf("%s ", temp_to_add->first_costumers_name);
      temp_to_add->done = 1;
      temp_to_add->next = NULL;

      pointer_temp_to_worker_assignment_head->next = temp_to_add;
      pointer_temp_to_worker_assignment_head = pointer_temp_to_worker_assignment_head->next;

      counter++;
    }
    printf("\n");

    /*
      reset all informat
      ion in boundle
    */
    boundles.order_time = 0;
    for (int i = 0; i < 10; i++) {
        boundles.costumers_name[i][0] = '\0';
    }
    boundles.books_ordered = 0;
    boundles.electronics_ordered = 0;

  }

}


/*
  This function is to determain if the order is complete or not. Then, it will delete the order that is complete from the linked-list
  and add the worker to the Queue Workers which means he is availabel

  *current_time is a pointer to the current time when the program is running so we can decide of the order is finished or not

  does not returne anything
 */
void order_completed_for_one(int *current_time){

  /*
    this pointer is to  Worker_assignment list
   */
  Worker_assignment *pointer_to_list_head = head_pointer_to_workers_assigned;

  /*
    this node is to delete the node once we the order is completed
   */
  Worker_assignment *pointer_to_delete;

  if (pointer_to_list_head != NULL) {

      while (pointer_to_list_head != NULL) {
        /*
        If Worker_assignment is not empty, then we go though the list and find the completed orders
         */
        if ((pointer_to_list_head->order_completion_time <= *current_time) && (pointer_to_list_head->done != 0) ) {
          /*
          print the orders that are completed
          insert the name of the worker that finished the order back to the end of the queue
          */
          printf("OrderCompletion ");
          printf("%d ", pointer_to_list_head->order_completion_time);
          printf("%s ", pointer_to_list_head->first_costumers_name);
          printf("\n");

          enqueue(pointer_to_list_head->worker_name);

          /*
          ->done = 0 means that the order is completed
          then the program goes to the next order to check if it's done
          free completed node
          */
          pointer_to_list_head->done = 0;
          pointer_to_list_head = pointer_to_list_head->next;
          free(pointer_to_delete);

        } else if ( ( pointer_to_list_head->next != NULL )&&(pointer_to_list_head->next->order_completion_time <= *current_time) && (pointer_to_list_head->done = 1)) {
          /*
          print the orders that are completed
          insert the name of the worker that finished the order back to the end of the queue
          */
          printf("OrderCompletion ");
          printf("%d ", pointer_to_list_head->next->order_completion_time);
          printf("%s ", pointer_to_list_head->next->first_costumers_name);
          printf("\n");

          enqueue(pointer_to_list_head->next->worker_name);

          /*
          ->done = 0 means that the order is completed
          then the program goes to the next order to check if it's done
          free completed node
          */
          pointer_to_list_head->done = 0;
          pointer_to_list_head->next = pointer_to_list_head->next->next;
           free(pointer_to_delete);
        }
        else {
          /*
          -if the next node does not satisfy the previous condition then we terminate the fonction
          */
          return;
        }

      }



  }

}


/*
  This function is to determain if the order is complete or not. Then, it will delete the order that is complete from the linked-list
  and add the worker to the Queue Workers which means he is availabel (used at the end to empty Worker_assignment linked list)

  *current_time is a pointer to the current time when the program is running so we can decide of the order is finished or not

  does not returne anything
 */
void order_completed_for_the_end(int *current_time){
  /*
    this pointer is to  Worker_assignment list
   */
  Worker_assignment *pointer_to_list_head = head_pointer_to_workers_assigned;

  /*
    this node is to delete the node once we the order is completed
   */
  Worker_assignment *pointer_to_delete;


  if (pointer_to_list_head != NULL) {

    while (pointer_to_list_head != NULL) {
      /*
      If Worker_assignment is not empty, then we go though the list and find the completed orders
       */
      if ( (pointer_to_list_head->order_completion_time > *current_time) && (pointer_to_list_head->done == 1) ) {
        /*
        print the orders that are completed
        insert the name of the worker that finished the order back to the end of the queue
        */
        printf("OrderCompletion ");
        printf("%d ", pointer_to_list_head->order_completion_time);
        printf("%s ", pointer_to_list_head->first_costumers_name);
        printf("\n");

        enqueue(pointer_to_list_head->worker_name);

        /*
        ->done = 0 means that the order is completed
        then the program goes to the next order to check if it's done
        free completed node
        */
        pointer_to_list_head->done = 0;
        pointer_to_delete = pointer_to_list_head;
        pointer_to_list_head = pointer_to_list_head->next;
        free(pointer_to_delete);

      }else if ( ( pointer_to_list_head->next != NULL )&&(pointer_to_list_head->next->order_completion_time <= *current_time) && (pointer_to_list_head->done = 1)) {
        /*
        print the orders that are completed
        insert the name of the worker that finished the order back to the end of the queue
        */
        printf("OrderCompletion ");
        printf("%d ", pointer_to_list_head->next->order_completion_time);
        printf("%s ", pointer_to_list_head->next->first_costumers_name);
        printf("\n");

        enqueue(pointer_to_list_head->next->worker_name);

        /*
        ->done = 0 means that the order is completed
        then the program goes to the next order to check if it's done
        free completed node
        */;
        pointer_to_list_head->done = 0;
        pointer_to_list_head->next = pointer_to_list_head->next->next;
        free(pointer_to_delete);
      }else {
        /*
        -if the next node does not satisfy the previous condition then we terminate the fonction
        */
        break;
      }

    }

  }

}


/*
  This function is to print the names of the workers who are assigned to orders and are working

  current_time is the last current time was scaned from the file

  does not returne anything
 */
void workers_assigned_list(int current_time) {

  /*
    this pointer is to  Worker_assignment list
   */
  Worker_assignment *pointer_to_list_head = head_pointer_to_workers_assigned;

  /*
    if the list is NOT empty, print the statemen and go to the while loop to print workers name
   */
  if (pointer_to_list_head != NULL) {
    printf("WorkerAssignmentList ");

    while ((pointer_to_list_head != NULL) ) {

      /*
        if the order compeletion time is greater than the current time then print the name of the worker and the name of the costumer
       */
      if ( (pointer_to_list_head->order_time <= current_time) && (pointer_to_list_head->order_completion_time > current_time )) {
        printf("%s", pointer_to_list_head->worker_name);
        printf(":");
        printf("%s", pointer_to_list_head->first_costumers_name);
      }

      /*
        if the next node's completion time is greater than corrent time too, Then print space to prepare for printing the nect worker:costumer name
        Then, go to the next node
       */
      if ( (pointer_to_list_head->next != NULL) && (pointer_to_list_head->next->order_time <= current_time) && (pointer_to_list_head->order_completion_time > current_time ) ) {
        printf(" ");
      }

      pointer_to_list_head = pointer_to_list_head->next;
    }
    printf("\n");

  }

}


/*
  This function is to print the names of the workers who are availabe

  does not returne anything
 */
void workers_available_list(){
  /*
    this pointer is to  Worker_assignment list
   */
  Workers *pointer_to_queue_front = front;

  /*
    If the worker's queue is NOT empty, then print  the statment AvailableWorkerList
   */
  if (pointer_to_queue_front != NULL) {
    printf("AvailableWorkerList ");

    while (pointer_to_queue_front != NULL) {
      /*
        while the queue pointer is not empty,
        print the workers name
        go to the next node in the queue
       */
      printf("%s ", pointer_to_queue_front->workers_name );
      pointer_to_queue_front = pointer_to_queue_front->next;
    }
    printf("\n");
  }

}


/*
  This function is to calculate the longiest that a worker took to finish an order

  returnes the number of minutes spent on the longest order
 */
int max_fulfillment_time(int last_max_fulfillment_time){

  /*
    this pointer is to  Worker_assignment list
   */
  Worker_assignment *pointer_to_list_head = head_pointer_to_workers_assigned;

  /*
    a variable that holds the value of the maxFulfillmentTime so it can be returne at the end of the function
   */
  int maxFulfillmentTime = 0;


  if (pointer_to_list_head != NULL) {
    while (pointer_to_list_head != NULL) {
      /*
        while the pointer to the list is not empty
        check if the (completion time - order time) is greater than the last last_max_fulfillment_time
        if true: store the result in maxFulfillmentTime
        if false: maxFulfillmentTime holds the same value is before
        Then, go the the next node

        Finally, returne the maxFulfillmentTime
       */
      if (last_max_fulfillment_time < (pointer_to_list_head->order_completion_time) - (pointer_to_list_head->order_time)) {
        maxFulfillmentTime = (pointer_to_list_head->order_completion_time) - (pointer_to_list_head->order_time);
      } else {
        maxFulfillmentTime = last_max_fulfillment_time;
      }
      pointer_to_list_head = pointer_to_list_head->next;
    }

  }

  return maxFulfillmentTime;
}
