/*

  Author: Thabet Alenezi
  Email: talenezi2018@my.fit.edu
  Course: CSE2010
  Section: 14
  Description: this implement the commands that are entered from an input file by using Skip List



 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "skiplist.c"
#include "fakeRandHeight.c"




/*
  Description for each function, parameter, and return value (if any)
 */
int main(int argc, char* argv[])
{

    DLL skipist[5]; // an array of pointers to DLL_Node, each index will be a level
    for (int i = 0; i < 5 ; i++) { // this loop is to initilize the array and make sure it has NULL in all of its levels
        skipist[i].head = NULL;
    }
    DLL_Node *temp_pointer; // temporary pointer to use later if needed
    DLL_Node *temp_pointer_to_previous_inserted = NULL; // temporary pointer to use later if needed, mainly used for insertion
    DLL_Node *temp_pointer_to_use; // temporary pointer to use later if needed


    char command[30]; // to store the first string from each line in the file which is the command
    int time; // to store time
    int time2; // another variable to store time if needed
    char activity[30]; // to store the action that happened at a spicifc time
    int level; // to store a randomized number from the function getRandHeight()
    int highest_level = 0; // to store the highest level we received from getRandHeight()

    FILE *input_file = fopen(argv[1], "r"); // a pointer to the file we need to read input from



    while(!feof(input_file)){ // while the input file is not empty, run the following line

        /*
            read
        */
        fscanf(input_file, "%s", command); // read the command

        /*
            if command is to add activity, then get the level where the new node will be inserted,
            if level is greater than highest_level, then update highest_level
            then scan time and activity, then print command, time, and activity to the screen
        */
        if ( strcmp(command,"AddActivity" ) == 0 ){
            level = getRandHeight();

            if (level > highest_level){
                highest_level = level;
            }

            fscanf(input_file, "%d %s", &time, activity);
            printf("%s %d %s", command, time, activity);

            /*
                loop until i reahces zero,
            */
            for (int i = level; i >= 0; i--) {

                int already_exist = 0; // to make sure it was inserted

                /*
                    for each level, make a node, then call the fucntion put() and star the value it returns in already_exist
                    if already_exist does not equal to one, then if the loop is at its first time, let the pointer to top  be NULL, if not, then adjust the top and bottom pointers
                    if already_exist quals to one it means the key already exist in the skip lsit
                 */
                temp_pointer = dll_create_node(time, activity, NULL, NULL, NULL, NULL); // make a new node that does not point to anything in preperation to insert it

                already_exist =  put(&skipist[i],time, activity, temp_pointer );
                if (already_exist != 1) {
                    if (i == level){
                        temp_pointer->top = NULL;
                        temp_pointer_to_previous_inserted = temp_pointer;
                    } else {
                        temp_pointer_to_previous_inserted->bottom = temp_pointer;
                        temp_pointer->top = temp_pointer_to_previous_inserted;
                    }
                } else {
                    printf(" existingTimeError");
                    break;
                }

            }
            printf("\n");


        } else if (  strcmp(command,"PrintSkipList" ) == 0 ){

            printf("%s\n", command); // print command

            DLL temp_node[5]; // a temporary varibale to hold the values of skiplist with out changing the original skip list

            for (int i = 5; i >= 0 ; i--) { // set tmep_node equals to skiplist
                temp_node[i].head = skipist[i].head;

            }

            for (int j = highest_level+1 ; j >= 0 ; j--) {
                /*
                    for each level, if the hea at that level is not NULL, then print the number of the level
                    while the head at this spicific level is not NULL, then print the linked list in this level
                 */
                if (temp_node[j].head != NULL) {
                    printf("(S%d)", j);

                    while ((temp_node[j].head != NULL)  ) {

                        if ((temp_node[j].head->data >9999 || temp_node[j].head->data < 0)){
                            temp_node[j].head = temp_node[j].head->next;
                        } else {

                            printf(" %d:%s", temp_node[j].head->data, temp_node[j].head->activity);
                            temp_node[j].head = temp_node[j].head->next;

                        }

                    }

                } else { // if the head at this spicific level is empty, then print empty

                    printf("(S%d) empty", j);

                }
                printf("\n");
            }

        } else if ( strcmp(command,"DisplayActivity" ) == 0 ){
            /*
                scan time from file.
                the,  call the function get() and store its value in temp_ponter_to_use
                if temp_pointer_to_use is not NULL, then print it values
             */
            fscanf(input_file, "%d", &time);

            temp_pointer_to_use = get(skipist, time, highest_level);

            if (temp_pointer_to_use != NULL){
                printf("%s %d %s\n", command, temp_pointer_to_use->data, temp_pointer_to_use->activity);
            } else {

            }




        } else if ( strcmp(command,"DisplayActivitiesBetweenTimes" ) == 0 ){
            DLL_Node *flore_node; // a temopary variable to store the flore node
            DLL_Node *ceiling_node;// a temporary variable to store the ceiling node is needed

            fscanf(input_file, "%d %d", &time, &time2); // scan the first and second time from the file

            /*
                call the function get() and store its value in flore_node,
                of flore_node is empt, them call the cuntion ceilingEntry() and store its value in flore_node
             */
            flore_node = get(skipist, time, highest_level);
            if (flore_node == NULL){
                flore_node = ceilingEntry(skipist, time, highest_level);
            }

            /*
                print the first necesarry elemtns of the line,
                then if the time in flore_node is greater than time2, it means there is no nodes between these values,
                if the time in flore_node is less than time2, then print all elemnts in the linked list until we reach time2
             */
            printf("DisplayActivitiesBetweenTimes %d %d", time, time2);
            if (flore_node->data > time2){
                printf(" none");

            } else {
                while ( (flore_node != NULL) && (flore_node->data <= time2)){
                    printf(" %d:%s", flore_node->data, flore_node->activity);
                    flore_node = flore_node->next;
                }
            }

            printf("\n");

        } else if ( strcmp(command,"DisplayActivitiesFromStartTime" ) == 0 ){
            fscanf(input_file, "%d", &time); // scan time from the input file

            /*
                call the function get() and store its value in temp_pointer_to_use,
                of temp_pointer_to_use is empty, them call the function ceilingEntry() and store its value in temp_pointer_to_use
             */
            temp_pointer_to_use = get(skipist, time, highest_level);
            if (temp_pointer_to_use == NULL){
                temp_pointer_to_use = ceilingEntry(skipist, time, highest_level);
            }

            /*
                print the first necesarry elemtns of the line,
                then if the flore_node is not empty, then print all elements until the last node of the linked list of level 0
                if the varibale flore_node is empty, then print none
             */
            printf("%s %d", command, time);
            if (temp_pointer_to_use != NULL){
                while ( temp_pointer_to_use != NULL ){
                    printf(" %d:%s", temp_pointer_to_use->data, temp_pointer_to_use->activity);
                    temp_pointer_to_use = temp_pointer_to_use->next;
                }

            } else {
                printf(" none");
            }
            printf("\n");



        } else if ( strcmp(command,"DisplayActivitiesToEndTime" ) == 0 ){

            temp_pointer = skipist[0].head; // pointer the the linked list in level 0

            fscanf(input_file, "%d", &time); // scan time from the input file

            /*
                call the function get() and store its value in temp_pointer_to_use,
                if temp_pointer_to_use is empty, then call the function ceilingEntry() and store its value in temp_pointer_to_use
             */
            temp_pointer_to_use = get(skipist, time, highest_level);
            if (temp_pointer_to_use == NULL){
                temp_pointer_to_use = ceilingEntry(skipist, time, highest_level);
            }

            /*
                print the first necesarry elemtns of the line,
                then if the time in flore_node is greater than time2, it means there is no nodes between these values,
                if the time in flore_node is less than time2, then print all elemnts in the linked list until we reach time2
             */
            printf("%s %d", command, time);
            if(temp_pointer->data <= temp_pointer_to_use->data) {

                while (temp_pointer->data <= temp_pointer_to_use->data ){
                    printf(" %d:%s", temp_pointer->data, temp_pointer->activity);
                    temp_pointer = temp_pointer->next;

                }
            } else {
                printf(" none");
            }
            printf("\n");


        } else if ( strcmp(command,"DeleteActivity" ) == 0 ){

            fscanf(input_file, "%d", &time);// scan time from input file

            /*
                call the function get() and store its value in temp_pointer_to_use,
                if temp_pointer_to_use is not empty, then call the funtion remove_node() to delete the node from the skip list
                if temp_pointer_to_use is empty, it means there is no nodes in the skip list with that key, then print noTimeError
             */
            temp_pointer_to_use = get(skipist, time, highest_level);
            if (temp_pointer_to_use != NULL){
                remove_node(skipist, time, highest_level);

                printf("%s %d\n", command, time);
            } else {
                printf("%s %d noTimeError\n", command, time);
            }


        } else if(strcmp(command,"DisplayAllActivities" ) == 0 ){
            printf("DisplayAllActivities"); // print the first elemnts of the line (command)

            /*
                set temp_pointer_to_use to be the linked list in level 0 of the skip list
                then, print all of its elements
             */
            temp_pointer_to_use = skipist[0].head;
            while ( temp_pointer_to_use != NULL ){
                printf(" %d:%s", temp_pointer_to_use->data, temp_pointer_to_use->activity);
                temp_pointer_to_use = temp_pointer_to_use->next;

            }
            printf("\n");
        }


        if (feof(input_file)) {
            break;
        }
        command[0] = '\0';

    }

    return 0;


}
