/** @file libpriqueue.c
 * * * */
 
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
     q->size = 0;
     q->comp = comparer;
     q->top = NULL;
     
 }
 
 
 /**
 Inserts the specified element into this priority queue.
 
 @param q a pointer to an instance of the priqueue_t data structure
 @param ptr a pointer to the data to be inserted into the priority queue
 @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
 int priqueue_offer(priqueue_t *q, void *ptr)
 {
     int count=0;
     priqueue_entry_t *test;
     test = q->top;
     int debug;
     debug = *(int*)ptr;
     priqueue_entry_t *new_entry;
     new_entry = malloc(sizeof(priqueue_entry_t));
     new_entry->value =  ptr;
     q->size++;
     priqueue_entry_t* current_entry;
     priqueue_entry_t* tmp;
     current_entry = NULL;
     if(q->top == NULL)
     {
         new_entry->next = NULL;
         q->top = new_entry;
         // test = q->top;
         // while(test!=NULL)
         // {
         //     printf("%d ",*test->value);
         //     test = test->next;
         // }
         
         // printf("\n");
         return 0;
     }
     else
     {
         current_entry = q->top;
         
         if(q->comp(ptr, current_entry->value) < 0)
         {
             new_entry->next = current_entry;
             q->top = new_entry;
             // test=q->top;
             // while(test!=NULL)
             // {
             //     printf("%d ",*test->value);
             //     test = test->next;
             // }
             // printf("\n");
             return 0;
         }
         
         while(current_entry != NULL)
         {
             //printf("loop\n");
             if(q->comp(ptr,current_entry->value) < 0)
             {
                 
                 new_entry->next = tmp->next;
                 tmp->next = new_entry;
                 // test=q->top;
                 // while(test!=NULL)
                 // {
                 //     printf("%d ",*test->value);
                 //     test = test->next;
                 // }
                 // printf("\n");
                 return count;
             }
             tmp = current_entry;
             current_entry = current_entry->next;
             count++;
         }
     }
     new_entry->next = NULL;
     tmp->next = new_entry;
     // test=q->top;
     // while(test!=NULL)
     // {
     //     printf("%d ",*test->value);
     //     test = test->next;
     // }
     // printf("\n");

     return count;
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
     if(q->size == 0)
     return NULL;
     else
     {
         priqueue_entry_t* first;
         first = q->top;
         return first->value;
     }
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
     if(q->size == 0)
     return NULL;
     else
     {
         void* return_ptr;
         priqueue_entry_t* first;
         first = q->top;
         return_ptr = first->value;
         q->top = first->next;
         q->size--;
         free(first);
         return return_ptr;
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
     int count=0;
     int size=q->size;
     
     
     if(index>size || index<0)
     return NULL;
     else
     {
         priqueue_entry_t* current_entry;
         current_entry = q->top;
         
         while(current_entry != NULL)
         {
             
             if(count==index)
             {
                 return current_entry->value;
             }
             current_entry = current_entry->next;
             count++;
         }
     }
     printf("error happens!\n");
     return NULL;
     
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
     int size = q->size;
     int return_value=0;
     if(size < 0)
     return -1;
     else
     {
         priqueue_entry_t* current_entry;
         priqueue_entry_t* tmp;
         tmp = NULL;
         current_entry = q->top;
         
         while(current_entry != NULL)
         {
             if(current_entry == q->top)
             {
                 if(ptr == current_entry->value)
                 {
                     return_value++;
                     q->top = current_entry->next;
                     q->size--;
                     free(current_entry);
                     current_entry = q->top;
                 }
                 else
                 current_entry = current_entry->next;
             }
             else
             {  
                 if(ptr == current_entry->value)
                 {
                     return_value++;
                     tmp->next=current_entry->next;
                     q->size--;
                     free(current_entry);
                 }
                 
                 
                 tmp = current_entry;
                 current_entry = tmp->next;
             }
             
             
             
         }
     }
     return return_value;
     
     
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
     int count=0;
     int size = q->size;
     if(index>size || index<0)
     return NULL;
     else
     {
         priqueue_entry_t* current_entry;
         current_entry = q->top;
         priqueue_entry_t* tmp;
         void* return_ptr;
         tmp = NULL;
         if(index == 0)
         {
             return_ptr = current_entry->value;
             q->size--;
             q->top = current_entry->next;
             free(current_entry);
             return return_ptr;
         }
         
         while(current_entry != NULL)
         {
             if(count==index)
             {

                 return_ptr = current_entry->value;
                 q->size--;
                 tmp->next = current_entry->next;
                 free(current_entry);
                 return return_ptr;;
             }

             tmp = current_entry;
             current_entry = current_entry->next;
             count++;
         }
     }
     return NULL;
     
 }
 
 
 /**
 Returns the number of elements in the queue.
 
 @param q a pointer to an instance of the priqueue_t data structure
 @return the number of elements in the queue
 */
 int priqueue_size(priqueue_t *q)
 {
     return q->size;
 }
 
 
 /**
 Destroys and frees all the memory associated with q.
 
 @param q a pointer to an instance of the priqueue_t data structure
 */
 void priqueue_destroy(priqueue_t *q)
 {
     priqueue_entry_t* current_entry;
     current_entry = q->top;
     priqueue_entry_t* tmp;
     while(current_entry!=NULL)
     {
         tmp = current_entry->next;
         free(current_entry);
         current_entry = tmp;
     }
     q->size = 0;
     q->top = NULL;
     
     
 }
 