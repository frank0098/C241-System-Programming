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
}dict;



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

void *head_pointer = NULL;


void *malloc(size_t size)
{
    size_t malloc_size;
    
    //malloc_size is the multiple of 8
    malloc_size=round_up(size);		
    
    //first time call,the head pointer is null
    if(head_pointer == NULL)	
    {
        //Allocate Enough Space
        void *heap_end;
        heap_end = sbrk(2*malloc_size);
        if(heap_end == NULL)
        return NULL;
        
        //Find the pointer to the new seg
        heap_end = heap_end+4;
        
        //malloc requested malloc_size + header for user
        size_t* tmp_head;
        tmp_head = (size_t*) heap_end;
        *tmp_head = malloc_size + 4;
        
        //Mask the last bit to keep track of a "used" seg
        *tmp_head = *tmp_head | 1;
        
        //The returned pointer to user;
        void *return_pointer;
        tmp_head++;
        return_pointer = (void*) tmp_head;
        
        //keep track of the head
        head_pointer = heap_end;
        
        //Then go to next free-block
        void *find_free_head;
        find_free_head = heap_end + malloc_size + 4;
        
        //Assign the header of the new free block
        dict header;
        header.size = 2*malloc_size - malloc_size - 4;
        header.prev = NULL;
        header.next = NULL;
        tmp_head = tmp_head + malloc_size;
        dict* tmp_head_pointer;
        tmp_head_pointer = (dict*) tmp_head;
        printf("segfault here\n");
        *tmp_head_pointer = header;
        printf("segfault here\n");
        head_pointer = tmp_head_pointer;
        
        return return_pointer;
        
    }
    
    //if it is not null
    
    dict* current_head;
    current_head = head_pointer;
    
    //Trasverse the linked list 
    while(1)
    {
        printf("current address is %p\n",current_head);
        //why this not working? *org->size
        size_t tmp_size;
        dict tmp_dict;
        tmp_dict = *current_head;
        tmp_size = tmp_dict.size;
        
        
        if(tmp_size >= malloc_size+4)
        {
            //The fragment is enough to malloc
            //pointer current_head now points to the head of the spot to malloc
            
            //find out the user_head
            void* find_user_head;
            find_user_head = (void*) current_head;

            //malloc requested malloc_size + header for user
        	size_t* tmp_head;
        	tmp_head = (size_t*) find_user_head;
        	*tmp_head = malloc_size + 4;
        
        	//Mask the last bit to keep track of a "used" seg
        	*tmp_head = *tmp_head | 1;

            find_user_head = find_user_head + 4;
            void* return_pointer;
            return_pointer = find_user_head;
            
            
            //Keep track of memory remained
            size_t memory_left;
            memory_left = tmp_size - malloc_size - 4; 
           
            
            
            if(tmp_size - malloc_size < 24)
            {
                return return_pointer;
            }
            else
            {
                
                //Find the next free block
                void* find_next_head;
                find_next_head = find_user_head;
                find_next_head = find_next_head + malloc_size;
                
                //Now find_next_head points to the start point of next availabe block
                dict* header_pointer;
                header_pointer = find_next_head;
                
                //If current node is already the last node in the linked list
                if(tmp_dict.next == NULL)
                {
       
                    //The header of the just-created free-block
                    dict header;
                    header.size = memory_left;
                    header.prev = current_head;
                    header.next = NULL;
                    *header_pointer = header;

                    //The header of the previous free block
        			dict previous_head;
        			dict* previous_head_pointer;
        			previous_head_pointer = tmp_dict.prev;
        			previous_head = *previous_head_pointer;

                    previous_head.size = previous_head.size;
                    previous_head.prev = previous_head.prev;
                    previous_head.next = header_pointer;
                    *previous_head_pointer = previous_head;
                }
                //if it is not the last node
                else
                {
                	//The header of the just-created free-block
                    dict header;
                    header.size = memory_left;
                    header.prev = current_head;
                    header.next = tmp_dict.next;
                    *header_pointer = header;

                    //The header of the previous free block
        			dict previous_head;
        			dict* previous_head_pointer;
        			previous_head_pointer = tmp_dict.prev;
        			previous_head = *previous_head_pointer;

                    previous_head.size = previous_head.size;
                    previous_head.prev = previous_head.prev;
                    previous_head.next = header_pointer;
                    *previous_head_pointer = previous_head;

                    //The header of the next free block
                    dict next_head;
        			dict *next_head_pointer;
        			next_head_pointer = tmp_dict.next;
        			next_head = *next_head_pointer;

                    next_head.size = next_head.size;
                    next_head.prev = header_pointer;
                    next_head.next = next_head.next;
                    *next_head_pointer = next_head;

                    
                    
                }
                
                return return_pointer;
            }  
        }  

        if(current_head->next == NULL)
    		break;
    
    	current_head = current_head->next;
    }
    
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
    dict to_free;
    dict *to_free_ptr;
    to_free_ptr = (dict*) tmp_ptr;
    to_free = *to_free_ptr;
    
    dict* prev_bloc_ptr;
    dict* next_bloc_ptr;
    
    prev_bloc_ptr = to_free.prev;
    next_bloc_ptr = to_free.next;
    
    dict prev_bloc;
    
    prev_bloc = *prev_bloc_ptr;
    
    prev_bloc.size = to_free.size + prev_bloc.size + 12;
    prev_bloc.prev = prev_bloc.prev;
    prev_bloc.next = next_bloc_ptr;
    
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
