/** @file alloc.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


/**
* Allocate space for array in memory
*
* Allocates a block of memory for an array of num elements, each of them size
* bytes long, and initializes all its bits to zero. The effective result is
* the allocation of an zero-initialized memory block of (num * size) bytes.
*
* @param num
*    Number of elements to be allocated.
* @param size
*    Size of elements.
*
* @return
*    A pointer to the memory block allocated by the function.
*
*    The type of this pointer is always void*, which can be cast to the
*    desired type of data pointer in order to be dereferenceable.
*
*    If the function failed to allocate the requested block of memory, a
*    NULL pointer is returned.
*
* @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
*/
void *calloc(size_t num, size_t size)
{
    /* Note: This function is complete. You do not need to modify it. */
    void *ptr = malloc(num * size);
    
    if (ptr)
    memset(ptr, 0x00, num * size);
    
    return ptr;
}


/**
* Allocate memory block
*
* Allocates a block of size bytes of memory, returning a pointer to the
* beginning of the block.  The content of the newly allocated block of
* memory is not initialized, remaining with indeterminate values.
*
* @param size
*    Size of the memory block, in bytes.
*
* @return
*    On success, a pointer to the memory block allocated by the function.
*
*    The type of this pointer is always void*, which can be cast to the
*    desired type of data pointer in order to be dereferenceable.
*
*    If the function failed to allocate the requested block of memory,
*    a null pointer is returned.
*
* @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
*/

typedef struct dict {
    size_t size;
    struct dict *prev;
    struct dict *next;
} *Dict;



size_t round_up(size_t size)
{
    size_t i=0;
    while(1)
    {
        if((size+i)%8==0)
        {
            return size+i;
            break;
        }
        i++;
    }
}

Dict *head_pointer = NULL;


void *malloc(size_t size)
{
    printf("initialization success! \n");

    size_t malloc_size;
    
    malloc_size=round_up(size);		//malloc_size is the multiple of 8
    
    printf("the head pointer is %p\n",head_pointer);

    if(head_pointer == NULL)	//first time call,initialization
    {

        void *heap_end;
        void *user_head;
        void *return_head;

       
        Dict* tmp_head;

        heap_end = sbrk(2*malloc_size);

        if(heap_end == NULL)
        	return NULL;

        heap_end = heap_end+4;
        Dict head;        
        head->size = size;
        head->prev = NULL;
        head->next = NULL;
		
        
        printf("dumped here!!!\n");
        printf("the first address is %p\n",heap_end);

        // Keep track of the head
        tmp_head = (Dict*) heap_end;
        *tmp_head = head;
        head_pointer = tmp_head;


        //Go to the user's first spot
        tmp_head++;
        user_head = (void*) tmp_head;
        return_head = user_head;
        printf("the user address is %p\n",return_head);
        size_t i=0;
        while(1)
        {
            i=i+4;
            user_head++;
            if(i == malloc_size)
            {
                user_head++;	//this is the start of the next block
                break;
            }
            
        }
        
        //The new head
        Dict next_head;
        next_head->size = 2*malloc_size - malloc_size;
        next_head->prev = *head_pointer;
        next_head->next = NULL;
        Dict* tmp_next_head;
        tmp_next_head = (Dict*) user_head;
        *tmp_next_head = next_head;
        



        tmp_head = (Dict*) heap_end;
        *tmp_head = head;
        head_pointer = tmp_head;

        




        // Assign the first
        
        head->next = tmp_next_head;
        tmp_head = (Dict*) heap_end;
        *tmp_head = head;
        printf("the stack address is %p\n",&head);


        printf("the address is %p\n",tmp_head);
        printf("the size is %zu\n",head->size);
        Dict usage;
        usage = *head_pointer;
        size_t* tmpptr;
        tmpptr = (size_t*) head_pointer;
        printf("the size is %zu\n",*tmpptr);

        return user_head;
        
    }




    Dict* org_head;
    org_head = head_pointer;

    printf("the head pointer is is %p\n",head_pointer);
    //Trasverse the linked list 
    while(1)
    {
    	printf("current address is %p\n",org_head);
    	//why this not working? *org->size
    	size_t tmp_size;
    	Dict tmp_dict;
    	tmp_dict = *org_head;
    	tmp_size = tmp_dict->size;

    	printf("over here\n");

    	Dict tmp_org_head;
    	tmp_org_head = *org_head;

        if(tmp_size >= malloc_size+12)
        {
        	//The fragment is enough to malloc
        	//pointer org_head now points to the head of the spot to malloc

            Dict* find_user_head;
            void* user_head;
            void* return_head;
            void* find_next_head;
            
            size_t memory_left;
            memory_left = tmp_size - malloc_size - 12; //Keep track of memory remained
            
            find_user_head = org_head;
            find_user_head++;
            user_head = (void*) find_user_head;
            return_head = user_head;
            
            if(tmp_size - malloc_size < 32)
            {
                
                return return_head;
            }
            else
            {
                size_t i=0;

                //Find next head
                while(1)
                {
                    i=i+4;
                    user_head++;
                    if(i == malloc_size)
                    {
                        user_head++;	//this is the start of the next block
                        break;
                    }
                    
                    //Now user head points to the start point of next availabe block
                    

                    //If current node is the last node in the linked list
                    if(tmp_dict->next == NULL)
                    {
					
						Dict next_head;
                        next_head->size = memory_left;
                        next_head->prev = org_head;
                        next_head->next = NULL;

                        Dict* tmp_next_head;
                    	tmp_next_head = (Dict*) user_head;
                        *tmp_next_head = next_head;


                        tmp_org_head->size = malloc_size;
                        tmp_org_head->prev = tmp_org_head->prev;
                        tmp_org_head->next = next_head;
                        *org_head = tmp_org_head;
                        
                        
                    }
                    //if it is not the last node
                    else
                    {
                    	Dict next_head;
                    	Dict following_head;

                    	//Find the node after the new-node
                    	Dict* following_head_pointer;
                    	following_head_pointer = tmp_org_head->next;
                        following_head = *following_head_pointer;

                        //Assign the new_node
                        next_head->size = memory_left;
                        next_head->prev = org_head;
                        next_head->next = following_head_pointer;

                        Dict* tmp_next_head;
                    	tmp_next_head = (Dict*) user_head;
                        *tmp_next_head = next_head;

                        //Assign the node after the new_node
                        following_head->size = following_head->size;
                        following_head->next = following_head->next;
                        following_head->prev = tmp_next_head;

                        //Assign the original node
                        tmp_org_head->size = malloc_size;
                        tmp_org_head->prev = tmp_org_head->prev;
                        tmp_org_head->next = next_head;
                        *org_head = tmp_org_head;

                        
                    }
                    
                    return return_head; 
                }  
            }  
        }
        if(tmp_org_head->next == NULL)
        	break;

        org_head = tmp_org_head->next;
    }
    //if no proper spots, expand the heap
    //org_head.size < malloc_size+12

   
    void* new_pointer;
    new_pointer = sbrk(4*malloc_size);
    if(new_pointer == NULL)
    	return NULL;

    void* user_head;
    void* return_head;
    Dict* tmp_find_last_head;
    tmp_find_last_head = org_head;
    tmp_find_last_head++;
    user_head = (void*) tmp_find_last_head;
    return_head = user_head;
    size_t i=0;
    while(1)
        {
            i=i+4;
            user_head++;
            if(i == malloc_size)
            {
                user_head++;	//this is the start of the next block
                break;
            }
            
        }

    Dict* last_head;
    last_head = (Dict*) user_head;

    Dict tmp_org_head;
    tmp_org_head = *org_head;
    tmp_org_head->size = 4*malloc_size + tmp_org_head->size;
    tmp_org_head->prev = tmp_org_head->prev;
    tmp_org_head->next = last_head;
    *org_head = tmp_org_head;

    Dict next_head;
    next_head->size = tmp_org_head->size - malloc_size;
    next_head->prev = org_head;
    next_head->next = NULL;

    return return_head;
    
    
    
}


/**
* Deallocate space in memory
*
* A block of memory previously allocated using a call to malloc(),
* calloc() or realloc() is deallocated, making it available again for
* further allocations.
*
* Notice that this function leaves the value of ptr unchanged, hence
* it still points to the same (now invalid) location, and not to the
* null pointer.
*
* @param ptr
*    Pointer to a memory block previously allocated with malloc(),
*    calloc() or realloc() to be deallocated.  If a null pointer is
*    passed as argument, no action occurs.
*/
void free(void *ptr)
{
    //"If a null pointer is passed as argument, no action occurs."
    if (!ptr)
    	return;

    void* tmp_ptr;
    tmp_ptr = tmp_ptr - 12;
    Dict to_free;
    Dict *to_free_ptr;
    to_free_ptr = (Dict*) tmp_ptr;
    to_free = *to_free_ptr;

    Dict* prev_bloc_ptr;
    Dict* next_bloc_ptr;

    prev_bloc_ptr = to_free->prev;
    next_bloc_ptr = to_free->next;

    Dict prev_bloc;

    prev_bloc = *prev_bloc_ptr;

    prev_bloc->size = to_free->size + prev_bloc->size + 12;
    prev_bloc->prev = prev_bloc->prev;
    prev_bloc->next = next_bloc_ptr;

    *prev_bloc_ptr = prev_bloc;


    
    return;
}


/**
* Reallocate memory block
*
* The size of the memory block pointed to by the ptr parameter is changed
* to the size bytes, expanding or reducing the amount of memory available
* in the block.
*
* The function may move the memory block to a new location, in which case
* the new location is returned. The content of the memory block is preserved
* up to the lesser of the new and old sizes, even if the block is moved. If
* the new size is larger, the value of the newly allocated portion is
* indeterminate.
*
* In case that ptr is NULL, the function behaves exactly as malloc, assigning
* a new block of size bytes and returning a pointer to the beginning of it.
*
* In case that the size is 0, the memory previously allocated in ptr is
* deallocated as if a call to free was made, and a NULL pointer is returned.
*
* @param ptr
*    Pointer to a memory block previously allocated with malloc(), calloc()
*    or realloc() to be reallocated.
*
*    If this is NULL, a new block is allocated and a pointer to it is
*    returned by the function.
*
* @param size
*    New size for the memory block, in bytes.
*
*    If it is 0 and ptr points to an existing block of memory, the memory
*    block pointed by ptr is deallocated and a NULL pointer is returned.
*
* @return
*    A pointer to the reallocated memory block, which may be either the
*    same as the ptr argument or a new location.
*
*    The type of this pointer is void*, which can be cast to the desired
*    type of data pointer in order to be dereferenceable.
*
*    If the function failed to allocate the requested block of memory,
*    a NULL pointer is returned, and the memory block pointed to by
*    argument ptr is left unchanged.
*
* @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
*/
void *realloc(void *ptr, size_t size)
{
    // "In case that ptr is NULL, the function behaves exactly as malloc()"
    if (!ptr)
    return malloc(size);
    
    // "In case that the size is 0, the memory previously allocated in ptr
    //  is deallocated as if a call to free() was made, and a NULL pointer
    //  is returned."
    if (!size)
    {
        free(ptr);
        return NULL;
    }
    
    
    
    return NULL;
}
