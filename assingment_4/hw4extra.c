/*

  Author: Thabet Alenezi
  Email: talenezi2018@my.fit.edu
  Course: CSE2010
  Section: 1
  Description:
 */

 /*  ****Explination*****
      1-since alpha_heap is another structure that stores the array max_heap in an ascending alphabatical order (sorted structure), then
      we can use binary search using binarySearch() function to find the element that we are looking for in a O(log(n)) time.

      2- we can update the original heap by searching for the element that has the the same name that we updated, we can use leaner search to accomplish
      this task in O(n). this is not the best appraoch, but is the one that i know that will work for sure.

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


/*
  a function that takes the current contents of the array and sort them to be a heap
  *heap: a pointer to the heap struct that holds all the bids information
  current_element: is the element that we need to compare with when the functuion is called
  heap_size: the size of the heap at the current moment

  the function does not return anything but it sorts the heap itself by the customer name
 */
void get_alpha_array(Heap *heap, int current_element, int heap_size);


/*
  a function that goes through the array and sort it alphabitaclly for later uses

  *heap: a pointer to the heap that contains the array that needs sorting
  array_size: is the size of the array

  does not return anything
 */
void sort_array(Heap *heap, int array_size);

/*
  a function that goes through the array and find the currect element that is wanted

  *heap: a pointer to the heap that contains the array that needs sorting
  low: the lowwer limit of the array
  high: the high limit of the array
  *name: the name of the customer that the function should look for

  return -1 if the element is not found
 */
int binarySearch(Heap *heap, int low, int high, char *name);



int main(int argc, char* argv[]){

    /*
    *heao: a pointer to the heap
     entered_element: is to read the bids from the file before it is entred in the heap
     size: is the size of the heap and it starts at zero
    */
    Heap *heap = malloc(sizeof(Heap));
    Heap *alpha_heap = malloc(sizeof(Heap));
    Info entered_element;
    int size = 0;
    int index_of_bid_to_update;

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
                alpha_heap->max_heap[size] = entered_element;
                size++;
            }else {
                heap->max_heap[size] = entered_element;
                alpha_heap->max_heap[size] = entered_element;
                size++;
            }

            /*
                loop through have of the heap and sort it by calling heap_sorting()
                then, print the information of the bid
            */
            for (int j = size/2; j >= 0; j--) {

                heap_sorting(heap, j, size);
                get_alpha_array(alpha_heap, j, size);
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
                        get_alpha_array(alpha_heap, j, size);
                    }
                    size--;

                }

            }

        } else if (strcmp(command, "UpdateBid") == 0){

            fscanf(input_file, "%d %s %lf %d", &current_time, customer_name, &offer, &quantity);
            strcpy(entered_element.name,customer_name );
            entered_element.offer = offer;
            entered_element.time = current_time;
            entered_element.quantity = quantity;

            sort_array(alpha_heap, size);

            index_of_bid_to_update = binarySearch(alpha_heap, 0, size, customer_name);

            if (index_of_bid_to_update != -1){
                alpha_heap->max_heap[index_of_bid_to_update].offer = entered_element.offer;
                alpha_heap->max_heap[index_of_bid_to_update].time = entered_element.time;
                alpha_heap->max_heap[index_of_bid_to_update].quantity = entered_element.quantity;


                printf("UpdateBid %d %s %.2f %d\n", alpha_heap->max_heap[index_of_bid_to_update].time, alpha_heap->max_heap[index_of_bid_to_update].name, alpha_heap->max_heap[index_of_bid_to_update].offer, alpha_heap->max_heap[index_of_bid_to_update].quantity );


            } else {
                printf("UpdateBid customerNotFound\n");
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

  the function does not return anything but it sorts the heap itself by the offer
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

/*
  a function that takes the current contents of the array and sort them to be a heap
  *heap: a pointer to the heap struct that holds all the bids information
  current_element: is the element that we need to compare with when the functuion is called
  heap_size: the size of the heap at the current moment

  the function does not return anything but it sorts the heap itself by the customer name
 */
void get_alpha_array(Heap *heap, int current_element, int heap_size){
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
        if (left < (heap_size) && ( (strcmp(heap->max_heap[left].name,heap->max_heap[current_element].name ) > 0) ) ) {

            largest = left;

        } else {

            largest = current_element;
        }

        /*
            if right is less than heap_size, and if the right offer is greater than the current offer,
            then, set largest to be right
        */
        if (right < (heap_size) && ( (strcmp(heap->max_heap[right].name, heap->max_heap[current_element].name ) > 0) )  ) {

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

            get_alpha_array(heap, largest, heap_size);

        }

    }

}


/*
  a function that goes through the array and sort it alphabitaclly for later uses

  *heap: a pointer to the heap that contains the array that needs sorting
  array_size: is the size of the array

  does not return anything
 */
void sort_array(Heap *heap, int array_size){
  /*
    temp: a temporarily variable to allow swapping of elements ehn needed
   */
    Info temp;

    /*
      loop through all elements and compare each one of them to the other elements,
      if it is smaller then swap the two elements
     */
    for (int i = 0; i < array_size; ++i) {

        for (int k = 0; k < array_size ; ++k) {

            if ( strcmp(heap->max_heap[k].name, heap->max_heap[i].name ) > 0 ) {

                temp = heap->max_heap[k];
                heap->max_heap[k] = heap->max_heap[i];
                heap->max_heap[i] = temp;

            }

        }

    }

}


/*
  a function that goes through the array and find the currect element that is wanted

  *heap: a pointer to the heap that contains the array that needs sorting
  low: the lowwer limit of the array
  high: the high limit of the array
  *name: the name of the customer that the function should look for

  return -1 if the element is not found
 */
int binarySearch(Heap *heap, int low, int high, char *name){
  /*
    a variabe to hold the middle value
   */
    int mid;
  /*
    if low is not greater than high, then set mid to be the middle point, else , return -1
    if if the name at the index (mid) is the currect name then return (mid)
   */
    if (high >= low) {
        mid = low + (high - low) / 2;

        if ( strcmp(heap->max_heap[mid].name , name) == 0 ){
            return mid;
        }


        // If element is smaller than mid, then
        // it can only be present in left subarray
        /*
          if the element is smaler then (mid) then call the function again with (mid -1) as the high value
          else, then call the function again with (mid + 1) as the low value
         */
        if (strcmp(heap->max_heap[mid].name , name) > 0){

          return binarySearch(heap, low, mid - 1, name);

        } else {

          return binarySearch(heap, mid + 1, high, name);

        }


    }

    return -1;
}
