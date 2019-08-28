/*

  Author: Thabet Alenezi
  Email: talenezi2018@my.fit.edu
  Course: CSE2010
  Section: 1
  Description: this program takes bids from a file and sort them in a heap structure to ease the process of getting the maximum offer of bids in
  a more efficient way


 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
  a structure that hold all information of bids
 */

typedef struct{
    int time;
    char name[20];
    double offer;
    int quantity;
}Info;

/*
  a structure that contains an array of bids
 */
typedef struct{
    Info max_heap[100];

}Heap;


/*
  a function that takes the current contents of the array and sort them to be a heap
  *heap: a pointer to the heap struct that holds all the bids information
  current_element: is the element that we need to compare with when the functuion is called
  heap_size: the size of the heap at the current moment

  the function does not return anything but it sorts the heap itself
 */
void heap_sorting(Heap *heap , int current_element, int heap_size);


int main(int argc, char* argv[]){

    /*
    *heao: a pointer to the heap
     entered_element: is to read the bids from the file before it is entred in the heap
     size: is the size of the heap and it starts at zero
    */
    Heap *heap = malloc(sizeof(Heap));
    Info entered_element;
    int size = 0;

    /*
    command: to read the command at the beginning of each line
     acceptable_min_price: is to store the minimum acceptable price of bids
     current_time: is to store the current time from each line
     customer_name: is to store the customer's name
     offer: is to store the offer of the bid
     quantity: is tp store the quantity demanded from the audience
    */
    char command[30];
    double acceptable_min_price;
    int current_time;
    char customer_name[20];
    double offer;
    int quantity;

    /*
        *input_file : is a pointer to the files opened
    */
    FILE *input_file = fopen(argv[1], "r");


    while(!feof(input_file) ){


        /*
            while the file is not empty:
            scan the command from the beginning of each line, then compare it to the possible inputs
        */
        fscanf(input_file, "%s", command);

        if (strcmp(command, "UpdateMinimumAcceptablePrice") == 0){
            /*
                update the current_time and acceptable_min_price by scaning them from the file
            */
            fscanf(input_file, "%d %lf", &current_time, &acceptable_min_price);
            printf("UpdateMinimumAcceptablePrice %d %.2f\n", current_time, acceptable_min_price);


        }else if (strcmp(command, "EnterBid") == 0){
            /*
                scan current_time, customer_name, offer, and quantity from the file, then put them in entered_element
            */
            fscanf(input_file, "%d %s %lf %d", &current_time, customer_name, &offer, &quantity);
            strcpy(entered_element.name,customer_name );
            entered_element.offer = offer;
            entered_element.time = current_time;
            entered_element.quantity = quantity;

            /*
                if the size is zero,  enter the bid at the root of the heap
                if not, enter the bid to the next available space
            */
            if (size == 0){
                heap->max_heap[size] = entered_element;
                size++;
            }else {
                heap->max_heap[size] = entered_element;
                size++;
            }

            /*
                loop through have of the heap and sort it by calling heap_sorting()
                then, print the information of the bid
            */
            for (int j = size/2; j >= 0; j--) {

                heap_sorting(heap, j, size);
            }

            printf("EnterBid %d %s %.2f %d\n",current_time, customer_name, offer, quantity );

        } else if (strcmp(command, "DisplayHighestBid") == 0){
            /*
                scan current_time from the file, then, print the highest bid which will be at the 0th index of the based array heap
            */
            fscanf(input_file, "%d", &current_time);

            printf("DisplayHighestBid %d %s %d %.2f %d\n", current_time, heap->max_heap[0].name,heap->max_heap[0].time,heap->max_heap[0].offer,heap->max_heap[0].quantity  );


        }else if(strcmp(command, "SellOneItem") == 0){
            /*
                scan current_time, then check if the maximum offer if more than acceptable_min_price.
                if is not more, print "HightestBiddingPriceIsTooLow"
                if it is, then sell one quantity at a time, then print the information of the transaction
            */

            fscanf(input_file, "%d", &current_time);

            if (heap->max_heap[0].offer < acceptable_min_price){
                printf("SellOneItem %d HightestBiddingPriceIsTooLow\n", current_time);
            }else {

                printf("SellOneItem %d %s %.2f\n", current_time, heap->max_heap[0].name, heap->max_heap[0].offer);
                heap->max_heap[0].quantity--;
                /*
                    when quantity gets to zero, set offer also to sero, and sort the heap to have the max offer to be the root.
                    then decrease the size of the heap
                */
                if (heap->max_heap[0].quantity == 0){
                    heap->max_heap[0].offer = 0;
                    for (int j = size/2; j >= 0; j--) {

                        heap_sorting(heap, j, size);
                    }
                    size--;

                }

            }

        }

        /*
            if file is empty, then end the loop.
            otherwise, set the first index of command to be '\0'
        */
        if (feof(input_file)) {
            break;
        }
        command[0] = '\0';

    }

    return 0;

}


/*
  a function that takes the current contents of the array and sort them to be a heap
  *heap: a pointer to the heap struct that holds all the bids information
  current_element: is the element that we need to compare with when the functuion is called
  heap_size: the size of the heap at the current moment

  the function does not return anything but it sorts the heap itself
 */
void heap_sorting(Heap *heap, int current_element, int heap_size) {
    /*
        temp: temporarily variable of type Info to hole information when swapping
        largest: to store the index of the larges offer between left node and right node
        left: the index of the left node
        right: the index of the right node
    */
    Info temp;
    int largest;
    int left = ((2 * current_element) + 1);
    int right = ((2 * current_element) + 2);

    /*
        if left is greater than the left index, then return
    */
    if (left >= heap_size) {

        return;
    } else {
        /*
            if left is less than heap_size, and if the left offer is greater than the current offer,
            then, set largest to be left
            else, set largest to be current_element
        */
        if (left < (heap_size) && heap->max_heap[left].offer > heap->max_heap[current_element].offer ) {

            largest = left;

        } else {

            largest = current_element;
        }

        /*
            if right is less than heap_size, and if the right offer is greater than the current offer,
            then, set largest to be right
        */
        if (right < (heap_size) && heap->max_heap[right].offer > heap->max_heap[largest].offer ) {

            largest = right;
        }

        if (largest != current_element) {
            /*
                is largest does not equal current_element, then swap the contents of largest and current_element
                then call the same function again with largest to be the current_element
            */
            temp = heap->max_heap[current_element];

            heap->max_heap[current_element] = heap->max_heap[largest];

            heap->max_heap[largest] = temp;

            heap_sorting(heap, largest, heap_size);

        }

    }

}
