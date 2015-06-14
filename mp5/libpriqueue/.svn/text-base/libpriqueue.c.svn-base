/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
  q -> head = NULL; 
  q -> size = 0; 
  q -> comp = comparer;
  
  

}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{

  //******** CREATE MEMORY FOR ENTRY AND UPDATE SIZE **************//
  priqueue_entry_t * add = malloc(sizeof(priqueue_entry_t)); 
  add -> data = ptr; 
  add -> next = NULL; 
  q->size ++;
  //******** EMPTY QUEUE ***********//

  if (q-> head == NULL){
    q->head = add;
    return 0;
  }

  //********* INSERT AT HEAD *********//

  int result; 
  result = q->comp(ptr, q->head->data); 
  //printf("THIS WAS THE RESULT FROM THE COMPARE FUNCTION %d\n", result);
  if (result < 0){
    add -> next = q->head; 
    q->head = add; 
    return 0; 
  }
  //WHAT IF 0

  //********* GENERAL CASE ***********/
  priqueue_entry_t * temp; 
  int counter = 1; 

  for(temp = q->head; temp -> next != NULL; temp = temp->next){
    result = q->comp(ptr, temp->next->data);
    if (result <0){
      add -> next = temp ->next; 
      temp -> next = add; 
      return counter; 
    }
    counter++; 
  }

  //printf("THIS IS THE VALUE IM LOOKING FOR %p\n",temp->next); 
  //******** INSERT AT END ********//
  temp -> next = add; 
  return counter; 
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
  if (q -> head == NULL){return NULL;}
  else {return q->head->data;}
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
  if (q->head == NULL){return NULL;}
  else{
    priqueue_entry_t * temp = q ->head; 
    q -> head = q -> head ->next; 
    void * back = temp -> data; 
    free(temp); 
    (q -> size)--; 
    return back; 
  }
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
  void * back = NULL; 
  if (index > q->size){
    back = NULL;
  }
  else{
    int i =0; 
    priqueue_entry_t * temp; 

    for (temp = q -> head; i<=index; temp= temp ->next){
      if (i == index){
        back =  temp -> data;
      }
      i++; 
    }
  
  }

  return back; 

}


/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{

  priqueue_entry_t * temp = q -> head; 
  int counter = 0; 
  int back = 0; 
  void * stupid; 

  //printf("This is the value of the ptr - %p\n", ptr); 

  while (temp != NULL){
    //printf("This is the %dth iteration\n", counter);
    //printf("This is the value of temp %p \n", temp);
    if(temp->data == ptr){
      //printf("DO I EVEN COME HERE?\n");
      temp = temp -> next; 
      stupid = priqueue_remove_at(q, counter); 
      //(q -> size)--; 
      counter --; 

      back ++; 
      //printf("This is the value of back - %d\n", back);
    }
    else {
      temp = temp -> next;
    }
    counter ++; 
  }
  return back;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{

  void * back = NULL; 
  priqueue_entry_t * temp;
  if (index > q -> size || index < 0 || (index == 0 && q-> size ==0)) {back = NULL;}
  else if (index == 0){
    back = q -> head -> data; 
    temp = q -> head; 
    q -> head = q -> head -> next; 
    free(temp); 
    (q -> size)--;
  }
  else {
    int i =0; 
    priqueue_entry_t * curr;

    for (curr = q -> head; i < index; curr = curr -> next){
      i++; 
    }


    temp = curr -> next; 
    if (temp != NULL){
    back = temp -> data; 
    curr -> next = temp -> next; 
    free(temp);}
    else {back = NULL;} 
    (q -> size)--;
  }

  return back; 
}


/**
  Returns the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
  return q ->size;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
  if (q == NULL){return;}

  priqueue_entry_t * curr = q -> head; 
  priqueue_entry_t * ahead = curr -> next; 

  while (ahead != NULL){
    free(curr);
    curr = ahead; 
    ahead = ahead -> next; 

  }
  free(curr);

}
