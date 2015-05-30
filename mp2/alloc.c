/** @file alloc.c */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


/**
* * Allocate space for array in memory
* *
* * Allocates a block of memory for an array of num elements, each of them size
* * bytes long, and initializes all its bits to zero. The effective result is
* * the allocation of an zero-initialized memory block of (num * size) bytes.
* *
* * @param num
* *    Number of elements to be allocated.
* * @param size
* *    Size of elements.
* *
* * @return
* *    A pointer to the memory block allocated by the function.
* *
* *    The type of this pointer is always void*, which can be cast to the
* *    desired type of data pointer in order to be dereferenceable.
* *
* *    If the function failed to allocate the requested block of memory, a
* *    NULL pointer is returned.
* *
* * @see http://www.cplusplus.com/reference/clibrary/cstdlib/calloc/
* */
void *calloc(size_t num, size_t size)
{
    /* Note: This function is complete. You do not need to modify it. */
    void *ptr = malloc(num * size);
    
    if (ptr)
    memset(ptr, 0x00, num * size);
    
    return ptr;
}


/**
* * Allocate memory block
* *
* * Allocates a block of size bytes of memory, returning a pointer to the
* * beginning of the block.  The content of the newly allocated block of
* * memory is not initialized, remaining with indeterminate values.
* *
* * @param size
* *    Size of the memory block, in bytes.
* *
* * @return
* *    On success, a pointer to the memory block allocated by the function.
* *
* *    The type of this pointer is always void*, which can be cast to the
* *    desired type of data pointer in order to be dereferenceable.
* *
* *    If the function failed to allocate the requested block of memory,
* *    a null pointer is returned.
* *
* * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
* */

typedef struct dict {
    int size;
    struct dict *prev;
    struct dict *next;
}dict;



int round_up(size_t size)
{
    int i=0;
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
void *tail_pointer = NULL;


void *malloc(size_t size)
{
    // //printf("\n");
    printf("malloc size is  %zu \n", size);
    int malloc_size;
    //malloc_size is the multiple of 8
    malloc_size=round_up(size);
    
    //first time call,the head pointer is null
    if(head_pointer == NULL)
    {
        //Allocate Enough Space
        void *heap_end;
        heap_end = sbrk(0);
        sbrk(4*malloc_size);
        tail_pointer = sbrk(0);
        
        if(heap_end == NULL)
        return NULL;
        //Find the pointer to the new seg
        heap_end = heap_end+4;
        
        //malloc requested malloc_size + header for user
        int* tmp_head;
        tmp_head = (int*) heap_end;
        *tmp_head = malloc_size + 4;
        
        //Mask the last bit to keep track of a "used" seg
        *tmp_head = *tmp_head | 1;
        
        //The returned pointer to user;
        void *return_pointer;
        void *user_ptr;
        user_ptr = (void*) tmp_head;
        user_ptr = user_ptr + 4;
        return_pointer = user_ptr;
        
        //Then go to next free-block
        void *find_free_head;
        find_free_head = user_ptr + malloc_size;
        
        //Assign the header of the new free block
        dict header;
        header.size = 2*malloc_size - malloc_size - 4;
        header.prev = NULL;
        header.next = NULL;
        
        dict* tmp_head_pointer;
        tmp_head_pointer = (dict*) find_free_head;
        
        *tmp_head_pointer = header;
        head_pointer = tmp_head_pointer;


        //printf("this address is  %p\n",find_free_head);	

    	//printf("previous address is  %p\n",header.prev);

    	//printf("next address is  %p\n",header.next);	

        printf("initiliaze finished: head pointer is at %p\n",head_pointer );

        printf("returned pointer  is at %p\n",return_pointer );
    	
        return return_pointer;
        
    }
    
    //if it is not null
    
    dict* current_head;
    current_head = head_pointer;
    
    //Trasverse the linked list
    while(1)
    {
        ////printf("current address is %p\n",current_head);
        //why this not working? *org->size
        int tmp_size;
        dict tmp_dict;
        tmp_dict = *current_head;
        tmp_size = tmp_dict.size;
        

            //printf("tmp_size %zu malloc_size %zu\n", tmp_size,malloc_size);
        
        if(tmp_size >= malloc_size + 4 || current_head->next == NULL)
        {
            
            //The fragment is enough to malloc
            //pointer current_head now points to the head of the spot to malloc
            
            //find out the user_head
            void* find_user_head;
            find_user_head = (void*) current_head;
            
            //malloc requested malloc_size + header for user
            int* tmp_head;
            tmp_head = (int*) find_user_head;
            *tmp_head = malloc_size + 4;
            
            //Mask the last bit to keep track of a "used" seg
            *tmp_head = *tmp_head | 1;
            
            find_user_head = find_user_head + 4;
            void* return_pointer;
            return_pointer = find_user_head;
            
            
            //Keep track of memory remained
            int memory_left;
            memory_left = tmp_size - malloc_size - 4;

            
            
            if(tmp_size - malloc_size < 24)
            {
                
                printf("  not enough space to \n");
                if(tmp_dict.next!=NULL)
                return return_pointer;
                else
                {
                    //Allocate New memory
                    int new_allocated_memory;
                    new_allocated_memory = 512 * size;
                    sbrk(new_allocated_memory);
                    tail_pointer = sbrk(0);
                    
                    //find out the next head
                    void* find_next_head;
                    find_next_head = find_user_head;
                    find_next_head = find_next_head + malloc_size;
                    
                    //Now find_next_head points to the start point of next availabe block
                    dict* header_pointer;
                    header_pointer = find_next_head;
                    
                    //The header of the just-created free-block
                    dict header;
                    header.size = tmp_dict.size + new_allocated_memory - malloc_size - 4;
                    header.prev = tmp_dict.prev; //question?
                    header.next = NULL;
                    *header_pointer = header;
                    
                    
                    //The header of the previous free block
                    dict previous_head;
                    dict* previous_head_pointer;
                    previous_head_pointer = tmp_dict.prev;
                    if(previous_head_pointer != NULL)
                    {
                        previous_head = *previous_head_pointer;
                        previous_head.size = previous_head.size;
                        previous_head.prev = previous_head.prev;
                        previous_head.next = header_pointer;
                        *previous_head_pointer = previous_head;
                    }
                    else
                    {
                        head_pointer = find_next_head;
                    }
                    return return_pointer;
                    
                    
                }
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
                    header.prev = tmp_dict.prev; //question?
                    header.next = NULL;
                    *header_pointer = header;
                    
                    //The header of the previous free block
                    dict previous_head;
                    dict* previous_head_pointer;
                    previous_head_pointer = tmp_dict.prev;
                    if(previous_head_pointer != NULL)
                    {
                        previous_head = *previous_head_pointer;
                        previous_head.size = previous_head.size;
                        previous_head.prev = previous_head.prev;
                        previous_head.next = header_pointer;
                        *previous_head_pointer = previous_head;
                    }
                    else
                    {
                        head_pointer = find_next_head;
                    }
                }
                //if it is not the last node
                else
                {
                	printf("seg here\n");
                    //The header of the just-created free-block
                    //printf(" next pointer is not null\n");
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
    printf("this happens\n");
    return NULL;
    
    
}








/**
* * Deallocate space in memory
* *
* * A block of memory previously allocated using a call to malloc(),
* * calloc() or realloc() is deallocated, making it available again for
* * further allocations.
* *
* * Notice that this function leaves the value of ptr unchanged, hence
* * it still points to the same (now invalid) location, and not to the
* * null pointer.
* *
* * @param ptr
* *    Pointer to a memory block previously allocated with malloc(),
* *    calloc() or realloc() to be deallocated.  If a null pointer is
* *    passed as argument, no action occurs.
* */
void free(void *ptr)
{

	//printf("\n free \n");
    //"If a null pointer is passed as argument, no action occurs."
    if (!ptr)
    return;

    void* h_ptr;
    h_ptr = ptr - 4;
    printf("the h_ptr address is %p\n",h_ptr );
    

    //Find next free block
    void* find_next;
    find_next = (void*) head_pointer;
    //printf("the head pointer is   %p\n",head_pointer);
    while(1)
    {
    	//printf("loop\n");
        int tmp_current_size;
        int* next_ptr;
        next_ptr = (int*) find_next;
        tmp_current_size = *next_ptr & ~1;
        
        if(!(*next_ptr & 0x1))
        {
            break;
        }
        find_next = find_next + tmp_current_size;
    }
    //printf("this address is  %p\n",find_next);	

    //Unmask the current pointer;
	int current_size;
	int *curr;
	curr = (int*) h_ptr;
	*curr = *curr & ~1;
	current_size = *curr;

    //next free head
    dict next_head;
    dict* next_head_pointer;
    next_head_pointer = (dict*) find_next;
    next_head = *next_head_pointer;

    //prev head
    dict* find_prev;
    find_prev = next_head.prev;

    //current head
    dict current_head;
    dict* current_head_ptr;
    current_head_ptr = (dict*) h_ptr;
    current_head.size = current_size;
    current_head.prev = next_head.prev;
    current_head.next = (dict*) find_next;

    //next head
    next_head.prev = current_head_ptr;
    *next_head_pointer = next_head;


    //coalescene possible
    if(find_next - h_ptr == current_size)
    	{
    		current_head.size = current_size +next_head.size;
    		current_head.next = next_head.next;
    	}
    *current_head_ptr = current_head;

    
    dict prev_head;
    //printf("previous address is  %p\n",find_prev);
    if(find_prev != NULL)
    {
    	prev_head = *find_prev;
    	prev_head.size = prev_head.size;
    	prev_head.prev = prev_head.prev;
    	prev_head.next = (dict*) h_ptr;
    	

    	//coalescene
    	if(find_prev + prev_head.size == h_ptr)
    	{
    		prev_head.size = prev_head.size + current_head.size;
    		prev_head.next = current_head.next;
    	}
    	*find_prev = prev_head;
    }
    else
    {
    	printf("oh yea\n");
    	head_pointer = (dict*) h_ptr;
    }

    printf("free finished:head pointer is at %p\n",head_pointer );


}


/**
* * Reallocate memory block
* *
* * The size of the memory block pointed to by the ptr parameter is changed
* * to the size bytes, expanding or reducing the amount of memory available
* * in the block.
* *
* * The function may move the memory block to a new location, in which case
* * the new location is returned. The content of the memory block is preserved
* * up to the lesser of the new and old sizes, even if the block is moved. If
* * the new size is larger, the value of the newly allocated portion is
* * indeterminate.
* *
* * In case that ptr is NULL, the function behaves exactly as malloc, assigning
* * a new block of size bytes and returning a pointer to the beginning of it.
* *
* * In case that the size is 0, the memory previously allocated in ptr is
* * deallocated as if a call to free was made, and a NULL pointer is returned.
* *
* * @param ptr
* *    Pointer to a memory block previously allocated with malloc(), calloc()
* *    or realloc() to be reallocated.
* *
* *    If this is NULL, a new block is allocated and a pointer to it is
* *    returned by the function.
* *
* * @param size
* *    New size for the memory block, in bytes.
* *
* *    If it is 0 and ptr points to an existing block of memory, the memory
* *    block pointed by ptr is deallocated and a NULL pointer is returned.
* *
* * @return
* *    A pointer to the reallocated memory block, which may be either the
* *    same as the ptr argument or a new location.
* *
* *    The type of this pointer is void*, which can be cast to the desired
* *    type of data pointer in order to be dereferenceable.
* *
* *    If the function failed to allocate the requested block of memory,
* *    a NULL pointer is returned, and the memory block pointed to by
* *    argument ptr is left unchanged.
* *
* * @see http://www.cplusplus.com/reference/clibrary/cstdlib/realloc/
* */
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
    void *new_ptr;
    new_ptr = malloc(size);
    memcpy(new_ptr,ptr,size);
    free(ptr);
    return new_ptr;
    
    
}
