/*

  Author: Thabet Alenezi
  Email: talenezi2018@my.fit.edu
  Course: CSE2010
  Section: 1
  Description: A program that finds the fastest path for robots to get from one station to another while they are working
               in a retailer store

 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

/*
  This is a structure to hold and manipulate inputted information

  station_name: to hold the name of the station
  x_coordinates: to hold x value
  y_coordinates: to hold y value
  *next: a pointer to the next node
 */
typedef struct Stations{
    char station_name[30];
    int x_coordinates;
    int y_coordinates;
    struct Stations *next;
} Stations;




/*
  this function is to add a line from the input file to the original list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void initiat_original_list(char name_of_station[30], int x_coordinates, int y_coordinates);



/*
  this function is to calculate the distance of the sequential

  returns the distance of the sequential list as a double
 */
double count_sequential_list_distance(void);


/*
  this function is to copy all nodes from sequential list to optimal list

  this function does not return anything
 */
void copy_sequential_to_optimal(void);

/*
  this function is to calculate the distance of the original list

  returns the distance of the original list as a double
 */
double calculate_original_list_distance(void);



/*
  this function is to find the fastest path to travel between all points
  it goes through all possibilities and find the best one

  elements_in_array: the number of elements in original list
  *best_distance: a pointer to best distance so the function can compare using it

  this function does not return anything
 */
void finding_fastest_path(int elements_in_list, double *best_distance);


/*
  this function is to copy all nodes from original list to unvisited list
  it does not copy the first nod which is "Packing_station"

  this function does not return anything
 */
void copy_original_to_unvisited(void);


/*
  this function is to add a node from original list to the unvisited places list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void add_to_unvisited_places_list(char station_name[30], int x_coordinates, int y_coordinates);


/*
  this function is to add a node from unvisited places list to the sequential list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void add_to_sequence_list(char station_name[30], int x_coordinates, int y_coordinates);


/*
  this function is to add a node from sequential list  to optimal list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void add_to_optimal_path_list(char station_name[30], int x_coordinates, int y_coordinates);


/*
  this function is to delete a node from the front of unvisited places list and add it to the end of sequential list

  this function does not return anything
 */
void add_from_unvisited_places_to_sequential(void);


/*
  this function is to delete a node from the end of sequential list and add it to the end of unvisited places list

  this function does not return anything
 */
void reset_unvisited_places_and_sequential(void);


/*
  this function is to add "Packing_station" node to the beginning and end of sequential list

  this function does not return anything
 */
void add_beginning_and_end_points(void);


/*
  this function is to delete "Packing_station" node to the beginning and end of sequential list

  this function does not return anything
 */
void delete_sequence_beginning_and_end(void);


/*
  this function is to check if the new sequential is alphabetically more ordered than optimal list

  return: Zero if the two lists are equal \\ One if the two lists are NOT equal
 */
int check_optimal_and_sequential_alphabatically(void);

/*
  this function is to reverse the order of nodes in optimal list

  this function does not return anything
 */
void reverse_optimal_list(void);

/*
  this function is to print the following:
    (Station name) (x Coordinates) (Y Coordinates) (The difference between the current node and the last node)

  this function does not return anything
 */
void printing_optimal_list(void);


/*
  *head_of_original_list: Pointer to the head of the original list
  *head_of_sequential_list: Pointer to the head of the sequential list
  *head_of_optimal_path: Pointer to the head of the optimal list
  *head_of_unvisited_places: Pointer to the head of the unvisited places list
 */
Stations *head_of_original_list = NULL;
Stations *head_of_sequential_list = NULL;
Stations *head_of_optimal_path = NULL;
Stations *head_of_unvisited_places = NULL;



int main(int argc, char* argv[]){

    /*
      description of declared variables
    */

    /*
      elements_entered: to hold the first number of the first line from the input file which is the number of elements in the file
      name_of_station: to hold the station name
      x_coordinates: to hold x value
      y_coordinates: to hold y value
      best_path_distance: it is a double set to be 10000.12 so we can compare it to optimal path distance and update it to be the new best_path_distance
    */
    int elements_entered;
    char name_of_station[30];
    int x_coordinates;
    int y_coordinates;
    double best_path_distance = 10000.12;

    /*
      a pointer to the inpute file

      reading the first line of the file that holds the number of elemets in the file
    */
    FILE *input_file = fopen(argv[1], "r" );
    fscanf(input_file, "%d", &elements_entered);

    /*
      for loop that go through all elements and add them to original list
    */
    for (int i = 0; i < elements_entered; i++) {
        fscanf(input_file, "%s %d %d", name_of_station, &x_coordinates, &y_coordinates);
        initiat_original_list(name_of_station , x_coordinates, y_coordinates);
    }

    // copy original to unvisited_places list
    /*
      copy all node from original list to unvisited places list except "Packing_station" node

      then, find all possibilities and store the best possibility in optimal list

      print the optimal list and the distance between each node and the node before it

      print the total distance of the best path
    */
    copy_original_to_unvisited();

    finding_fastest_path(elements_entered, &best_path_distance);

    printing_optimal_list();
    printf("%.2f\n", best_path_distance);

}


/*
  this function is to add a line from the input file to the original list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void initiat_original_list(char name_of_station[30], int x_coordinates, int y_coordinates) {

    /*
     a pointer to the head of the original list
    */
    Stations *pointer_to_original_head;

    /*
     malloc a temporary pointer to hold information and assign all information to this pointer
    */
    Stations *temporary_to_hold_info = malloc(sizeof(Stations));

    strcpy(temporary_to_hold_info->station_name, name_of_station);
    temporary_to_hold_info->x_coordinates = x_coordinates;
    temporary_to_hold_info->y_coordinates = y_coordinates;
    temporary_to_hold_info->next = NULL;

    /*
     if original list is empty, then set it equal to temporary_to_hold_info

     else : set pointer_to_original_head to the head of the list, then go to the end of the list
     and add temporary_to_hold_info to the end of it
    */
    if (head_of_original_list == NULL) {

        head_of_original_list = temporary_to_hold_info;
    } else {

        pointer_to_original_head = head_of_original_list;

        while (pointer_to_original_head->next != NULL) {
            pointer_to_original_head = pointer_to_original_head->next;
        }

        pointer_to_original_head->next = temporary_to_hold_info;

    }

}

/*
  this function is to calculate the distance of the sequential

  returns the distance of the sequential list as a double
 */
double count_sequential_list_distance(void) {

    /*
     a pointer to the head of sequential list

     distance: to store the distance so it could be returned later
    */
    Stations *pointer_to_sequential_head = head_of_sequential_list;
    double distance = 0;

    /*
     as long as the pointer to sequential list and the next node are not empty
     calculate the distance between them and add it to the variable "distance"

     then return "distance"
    */
    if (pointer_to_sequential_head != NULL) {
        while (pointer_to_sequential_head->next != NULL) {

            distance += sqrt( (double) pow( (double)pointer_to_sequential_head->next->x_coordinates - pointer_to_sequential_head->x_coordinates, 2 ) + pow( (double) pointer_to_sequential_head->next->y_coordinates - pointer_to_sequential_head->y_coordinates, 2) );

            pointer_to_sequential_head = pointer_to_sequential_head->next;
        }

    }

    return distance;


}


/*
  this function is to copy all nodes from sequential list to optimal list

  this function does not return anything
 */
void copy_sequential_to_optimal(void) {

    /*
     a pointer to the head of sequential list

     Set optimal list to NULL to prepare it to hold new information
    */
    Stations *pointer_to_sequential_head = head_of_sequential_list;
    head_of_optimal_path = NULL;


    /*
     if sequential list is not empty

     then go to every node it it and call add_to_optimal_path_list() to add the node to optimal list

     then update current node in sequential
    */
    if (pointer_to_sequential_head != NULL) {

        while (pointer_to_sequential_head != NULL) {

            add_to_optimal_path_list(pointer_to_sequential_head->station_name, pointer_to_sequential_head->x_coordinates, pointer_to_sequential_head->y_coordinates);

            pointer_to_sequential_head = pointer_to_sequential_head->next;
        }

    }

}

/*
  this function is to calculate the distance of the original list

  returns the distance of the original list as a double
 */
double calculate_original_list_distance(void) {

    /*
      a pointer to the head of the original list
      a pointer that holds the first node in original list

      a variable "distance" to add the distance between node to it and return it in the end
    */
    Stations *pointer_to_original_head = head_of_original_list;
    Stations *pointer_to_first_node_in_list = head_of_original_list;
    double distance = 0;

    /*
      if original head is not empty, and while the head and the next node are not empty
      then calculate the distance between them and add them to distance

      in the end we calculate the "distance" between the last node and the first node using pointer_to_first_node_in_list then add it to "distance

      return "distance"
    */
    if (pointer_to_original_head != NULL) {
        while ((pointer_to_original_head != NULL) && (pointer_to_original_head->next != NULL)) {
            distance += sqrt(  pow( (double)pointer_to_original_head->next->x_coordinates - pointer_to_original_head->x_coordinates, 2 ) + pow( (double) pointer_to_original_head->next->y_coordinates - pointer_to_original_head->y_coordinates, 2) );

            pointer_to_original_head = pointer_to_original_head->next;
        }

        distance += sqrt(  pow( (double)pointer_to_first_node_in_list->x_coordinates - pointer_to_original_head->x_coordinates, 2 ) + pow( (double) pointer_to_first_node_in_list->y_coordinates - pointer_to_original_head->y_coordinates, 2) );

    }

    return distance;
}


/*
  this function is to find the fastest path to travel between all points
  it goes through all possibilities and find the best one

  elements_in_array: the number of elements in original list
  *best_distance: a pointer to best distance so the function can compare using it

  this function does not return anything
 */
void finding_fastest_path(int elements_in_list, double *best_distance) {
    /*
      original_distance: hold the total distance of original list
      sequential_distance: hold the total distance of sequential list
      temp_distance_of_new_sequential: holds the new sequential list distance
    */
    double original_distance = calculate_original_list_distance();
    double sequential_distance = count_sequential_list_distance();
    double temp_distance_of_new_sequential;

    /*
      for loop to try every combination possibly
    */
    for (int i = 0; i < elements_in_list ; i++) {
        /*
         add one node to the sequential from unvisited places list.
         if i ==1 then add "packing_station" to the end and beginning of sequential list
         Then, calculate sequential distance
         */
        add_from_unvisited_places_to_sequential();
        if (elements_in_list == 1) {

            add_beginning_and_end_points();
            temp_distance_of_new_sequential = count_sequential_list_distance();

            /*
             if the new sequential distance is less then *best_distance, Then we check if new sequential is more alphabetically ordered than optimal list
             if optimal list is empty, then just copy sequential to optimal, and update *best_distance
             */
            if ( (temp_distance_of_new_sequential <= (*best_distance) )) {
                int is_less = check_optimal_and_sequential_alphabatically();

                if (head_of_optimal_path == NULL) {
                    copy_sequential_to_optimal();

                    *best_distance = temp_distance_of_new_sequential;
                }
                /*
                 if sequential list is more alphabetically ordered than optimal list, then copy sequential to optimal list
                 and update *best_distance

                 then delete "packing_station" nodes from the beginning and end of sequential
                 */
                if (  is_less > 0 ) {
                    copy_sequential_to_optimal();
                    reverse_optimal_list();
                    *best_distance = temp_distance_of_new_sequential;
                }

            }

            delete_sequence_beginning_and_end();

            /*
             if i != 1, then we go to the recursive case and we call the function with elements_in_list minus one

             in the end of the for loop reset sequential list and unvisited places list
             */
        } else {
            finding_fastest_path(elements_in_list - 1, best_distance);
        }

        reset_unvisited_places_and_sequential(); // problem in this function
    }

}


/*
  this function is to copy all nodes from original list to unvisited list
  it does not copy the first nod which is "Packing_station"

  this function does not return anything
 */
void copy_original_to_unvisited(void) {

    /*
     a pointer to original head
     */
    Stations *pointer_to_original_head = head_of_original_list;

    /*
     if original head is not empty, and while the pointer to original head is not empty,
     then add all node from original to unvisited places list using add_to_unvisited_places_list(), except "packing_station" node
     */
    if (pointer_to_original_head != NULL) {
        while (pointer_to_original_head != NULL) {

            if ( strcmp(pointer_to_original_head->station_name, "packing_station") == 0 ) {
                pointer_to_original_head = pointer_to_original_head->next;
            } else {

                add_to_unvisited_places_list(pointer_to_original_head->station_name, pointer_to_original_head->x_coordinates, pointer_to_original_head->y_coordinates );

                pointer_to_original_head = pointer_to_original_head->next;

            }

        }

    }

}


/*
  this function is to add a node from original list to the unvisited places list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void add_to_unvisited_places_list(char station_name[30], int x_coordinates, int y_coordinates) {
    /*
     a pointer to unvisited places list
     a temporally pointer to hold information entered

     set temporally pointer to hold the new information
     */
    Stations *pointer_to_unvisited_places_head = head_of_unvisited_places;
    Stations *temporary_to_hold_info = malloc(sizeof(Stations));

    strcpy(temporary_to_hold_info->station_name, station_name);
    temporary_to_hold_info->x_coordinates = x_coordinates;
    temporary_to_hold_info->y_coordinates = y_coordinates;
    temporary_to_hold_info->next = NULL;


    /*
     if unvisited places list is empty, then add to the beginning  of it
     if it is not empty, then add the node to the end of it
     */
    if (head_of_unvisited_places == NULL) {
        head_of_unvisited_places = temporary_to_hold_info;
    } else {

        while (pointer_to_unvisited_places_head->next != NULL) {
            pointer_to_unvisited_places_head =  pointer_to_unvisited_places_head->next;
        }
        pointer_to_unvisited_places_head->next = temporary_to_hold_info;

    }

}


/*
  this function is to add a node from unvisited places list to the sequential list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void add_to_sequence_list(char station_name[30], int x_coordinates, int y_coordinates) {

    /*
     a pointer to sequential list
     a temporally pointer to hold information entered

     set temporally pointer to hold the new information
     */
    Stations *pointer_to_sequential_head = head_of_sequential_list;
    Stations *temporary_to_hold_info = malloc(sizeof(Stations));

    strcpy(temporary_to_hold_info->station_name, station_name);
    temporary_to_hold_info->x_coordinates = x_coordinates;
    temporary_to_hold_info->y_coordinates = y_coordinates;
    temporary_to_hold_info->next = NULL;

    /*
     if sequential list is empty, then add to the beginning  of it
     if it is not empty, then add the node to the end of it
     */
    if (head_of_sequential_list == NULL) {
        head_of_sequential_list = temporary_to_hold_info;
    } else {

        while (pointer_to_sequential_head->next != NULL) {
            pointer_to_sequential_head =  pointer_to_sequential_head->next;
        }
        pointer_to_sequential_head->next = temporary_to_hold_info;

    }

}


/*
  this function is to add a node from sequential list  to optimal list

  name_of_station: is the name of the station
  x_coordinates: x coordinates of the station
  y_coordinates: y coordinates of the station

  this function does not return anything
 */
void add_to_optimal_path_list(char station_name[30], int x_coordinates, int y_coordinates) {

    /*
     a pointer to optimal path list
     a temporally pointer to hold information entered

     set temporally pointer to hold the new information
     */
    Stations *pointer_to_optimal_head = head_of_optimal_path;
    Stations *temporary_to_hold_info = malloc(sizeof(Stations));

    strcpy(temporary_to_hold_info->station_name, station_name);
    temporary_to_hold_info->x_coordinates = x_coordinates;
    temporary_to_hold_info->y_coordinates = y_coordinates;
    temporary_to_hold_info->next = NULL;

    /*
     if optimal path list is empty, then add to the beginning  of it
     if it is not empty, then add the node to the end of it
     */
    if (head_of_optimal_path == NULL) {
        head_of_optimal_path = temporary_to_hold_info;
    } else {

        while (pointer_to_optimal_head->next != NULL) {
            pointer_to_optimal_head =  pointer_to_optimal_head->next;
        }
        pointer_to_optimal_head->next = temporary_to_hold_info;

    }

}


/*
  this function is to delete a node from the front of unvisited places list and add it to the end of sequential list

  this function does not return anything
 */
void add_from_unvisited_places_to_sequential(void) {

    /*
     temporally pointer to hold the address of the node we want to delete
     */
    Stations *temp_to_delete;

    /*
     if the head of unvisited places is not empty, then add the first node from unvisited places to sequential list,
     and set temp_to_delete =  the first node in unvisited places list, then update unvisited places list to next
     free temp_to_delete
     */
    if (head_of_unvisited_places != NULL) {

        add_to_sequence_list(head_of_unvisited_places->station_name, head_of_unvisited_places->x_coordinates, head_of_unvisited_places->y_coordinates);

        temp_to_delete = head_of_unvisited_places;
        head_of_unvisited_places = head_of_unvisited_places->next;
        free(temp_to_delete);


    }

}


/*
  this function is to delete a node from the end of sequential list and add it to the end of unvisited places list

  this function does not return anything
 */
void reset_unvisited_places_and_sequential(void) {

    /*
     a pointer to sequential list head
     temporally pointer to hold the address of the node we want to delete
     */
    Stations *pointer_to_sequential_head = head_of_sequential_list;
    Stations *temp_to_delete;

    /*
     is sequential list is not empty, and if the next next node is not empty too, then go to the end of sequential list
     set temp_to_delete to be the last node of sequential list,
     add the end of sequential list to unvisited places list using add_to_unvisited_places_list()
     set the next of sequential list node to NULL
     then free temp_to_delete
     */
    if (pointer_to_sequential_head != NULL) {

        if (pointer_to_sequential_head->next != NULL) {

            while (pointer_to_sequential_head->next->next != NULL) {
                pointer_to_sequential_head = pointer_to_sequential_head->next;
            }

            temp_to_delete = pointer_to_sequential_head->next;

            add_to_unvisited_places_list(pointer_to_sequential_head->next->station_name, pointer_to_sequential_head->next->x_coordinates, pointer_to_sequential_head->next->y_coordinates);
            pointer_to_sequential_head->next = NULL;
            free(temp_to_delete);

        /*
         if sequential next is empty, then add the current node to unvisited places list using add_to_unvisited_places_list()
         and set the head of sequential list to NULL
         */
        } else {
            add_to_unvisited_places_list(pointer_to_sequential_head->station_name, pointer_to_sequential_head->x_coordinates, pointer_to_sequential_head->y_coordinates);

            head_of_sequential_list = NULL;
        }

    }

}

/*
  this function is to add "Packing_station" node to the beginning and end of sequential list

  this function does not return anything
 */
void add_beginning_and_end_points(void) {
    /*
     a pointer to sequential list head
     temporally pointer to hold information
     */
    Stations *pointer_to_sequential_head = head_of_sequential_list;
    Stations *temporary_to_hold_info = malloc(sizeof(Stations));

    /*
     if sequential list is not empty
     then, set temporally pointer to to have the current original list nod which is "packing_station" node
     */
    if (pointer_to_sequential_head != NULL) {

        strcpy(temporary_to_hold_info->station_name, head_of_original_list->station_name);
        temporary_to_hold_info->x_coordinates = head_of_original_list->x_coordinates;
        temporary_to_hold_info->y_coordinates = head_of_original_list->y_coordinates;
        temporary_to_hold_info->next = NULL;

        /*
         add "packing_station" to the end of sequential list using add_to_sequence_list()
         then , add "packing_station" node to the beginning of sequential list as well
         */
        add_to_sequence_list(head_of_original_list->station_name, head_of_original_list->x_coordinates, head_of_original_list->y_coordinates);

        temporary_to_hold_info->next = head_of_sequential_list;
        head_of_sequential_list = temporary_to_hold_info;

    }

}


/*
  this function is to delete "Packing_station" node to the beginning and end of sequential list

  this function does not return anything
 */
void delete_sequence_beginning_and_end(void){

    /*
     a pointer to sequential list head
     temporally pointer to hold first node of sequential
     temporally pointer to hold last node of sequential
     */
    Stations *pointer_to_sequential_head = head_of_sequential_list;
    Stations *temp_to_delete_beginning;
    Stations *temp_to_delete_end;


    /*
     if sequential head is not empty, then set temp_to_delete_beginning to hold the first node, and move sequential head to the next node
     then, free temp_to_delete_beginning
     */
    if (pointer_to_sequential_head != NULL) {
        temp_to_delete_beginning = head_of_sequential_list;
        head_of_sequential_list = head_of_sequential_list->next;
        free(temp_to_delete_beginning);

        /*
         if sequential head is not empty, then, go to the end of sequential and set temp_to_delete_end to the last node,
         set next in sequential to NULL
         then, free temp_to_delete_end
         */
        if (head_of_sequential_list != NULL) {

            while (pointer_to_sequential_head->next->next != NULL) {
                pointer_to_sequential_head = pointer_to_sequential_head->next;
            }
            temp_to_delete_end = pointer_to_sequential_head->next;
            pointer_to_sequential_head->next = NULL;
            free(temp_to_delete_end);
        }

    }

}


/*
  this function is to check if the new sequential is alphabetically more ordered than optimal list

  return: Zero if the two lists are equal \\ One if the two lists are NOT equal
 */
int check_optimal_and_sequential_alphabatically(void) {

    /*
     a pointer to sequential list head
     a pointer to optimal path list head
     is_less: a variable to hold True = 1 , ot False = 0
     */
    Stations *pointer_to_sequential_head = head_of_sequential_list;
    Stations *pointer_to_optimal_head = head_of_optimal_path;
    int is_less = 0;


    /*
     sequential list is not empty, and optimal path list not empty,
     then while they are not empty, go through them and compare the two strings of every node
     if sequential list string is less than optimal path string, then is_less = 1 and return it
     if sequential list string is less than optimal path string, then is_less still equal 0 and return it
     */
    if ( (pointer_to_sequential_head != NULL) && (pointer_to_optimal_head != NULL) ) {

        while ((pointer_to_sequential_head != NULL) && (pointer_to_optimal_head != NULL)) {

            if ( strcmp(pointer_to_sequential_head->station_name, pointer_to_optimal_head->station_name) <= 0  ) {
                is_less = 1;
                return is_less;

            }

            pointer_to_sequential_head = pointer_to_sequential_head->next;
            pointer_to_optimal_head = pointer_to_optimal_head->next;
        }
    }

    return is_less;
}


/*
  this function is to reverse the order of nodes in optimal list

  this function does not return anything
 */
void reverse_optimal_list(void) {
    /*
     current_node: to hold the optimal path current node
     previous_node: is set to be NULL but it will hold the current next as a previous
     next_node: is next_node set to be NULL but it will hold current next node
     */
    Stations *current_node = head_of_optimal_path;
    Stations *previous_node = NULL;
    Stations *next_node = NULL;

    /*
     if current_node is not NULL, then set next_node to be current next node
     and current_node next equals previous_node

     Then, update previous_node to be current_node
     and  current_node gets updated to next_node

     lastly, set the head of the list to be previous_node
     */
    while (current_node != NULL){

        next_node = current_node->next;
        current_node->next = previous_node;

        previous_node = current_node;
        current_node = next_node;
    }
    head_of_optimal_path = previous_node;
}


/*
  this function is to print the following:
    (Station name) (x Coordinates) (Y Coordinates) (The difference between the current node and the last node)

  this function does not return anything
 */
void printing_optimal_list(void) {

    /*
     a pointer to the optimal path list
     previous_x: to hold the value of x coordinates
     previous_y: to hold the value of x coordinates
     */
    Stations *pointer_to_optimal_head = head_of_optimal_path;
    int previous_x;
    int previous_y;


    /*
     if optimal head is not empty, then print station name, x value ,and y value
     print that difference in distance = 0.00 because it is the beginning of the list

     then save c and y values in previous_x and previous_y

     then, update the pointer of optimal list to go to the next node
     */
    if (pointer_to_optimal_head != NULL) {
        printf("%s ", pointer_to_optimal_head->station_name);
        printf("%d ", pointer_to_optimal_head->x_coordinates);
        printf("%d ", pointer_to_optimal_head->y_coordinates);
        printf("%.2f\n", 0.00);

        previous_x = pointer_to_optimal_head->x_coordinates;
        previous_y = pointer_to_optimal_head->y_coordinates;


        pointer_to_optimal_head = pointer_to_optimal_head->next;

        /*
         if the pointer to optimal path list is NOT empty,

         then, while the pointer to optimal list pointer is not empty, print nodes information,,
         update previous_x and previous_y to hold x and y values of the current node,,

         then update the node to optimal path list to go to next node
         */
        if ( pointer_to_optimal_head != NULL ) {

            while (pointer_to_optimal_head != NULL) {

                printf("%s ", pointer_to_optimal_head->station_name);
                printf("%d ", pointer_to_optimal_head->x_coordinates);
                printf("%d ", pointer_to_optimal_head->y_coordinates);
                printf("%.2f\n", sqrt( pow( (double)pointer_to_optimal_head->x_coordinates - previous_x, 2 ) + pow( (double) pointer_to_optimal_head->y_coordinates - previous_y, 2) ));

                previous_x = pointer_to_optimal_head->x_coordinates;
                previous_y = pointer_to_optimal_head->y_coordinates;

                pointer_to_optimal_head = pointer_to_optimal_head->next;
            }

        }

    }

}
