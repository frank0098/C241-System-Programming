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
* *    A posize_ter to the memory block allocated by the function.
* *
* *    The type of this posize_ter is always void*, which can be cast to the
* *    desired type of data posize_ter in order to be dereferenceable.
* *
* *    If the function failed to allocate the requested block of memory, a
* *    NULL posize_ter is returned.
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
* * Allocates a block of size bytes of memory, returning a posize_ter to the
* * beginning of the block.  The content of the newly allocated block of
* * memory is not initialized, remaining with indeterminate values.
* *
* * @param size
* *    Size of the memory block, in bytes.
* *
* * @return
* *    On success, a posize_ter to the memory block allocated by the function.
* *
* *    The type of this posize_ter is always void*, which can be cast to the
* *    desired type of data posize_ter in order to be dereferenceable.
* *
* *    If the function failed to allocate the requested block of memory,
* *    a null posize_ter is returned.
* *
* * @see http://www.cplusplus.com/reference/clibrary/cstdlib/malloc/
* */

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
            break;
        }
        i++;
    }
    if(size+i < 24)
    	return 24;
    else 
     	return size+i;
}

void *head_posize_ter = NULL;
void *tail_posize_ter = NULL;


void *malloc(size_t size)
{
    
    //getchar();
    size_t malloc_size;
    //malloc_size is the multiple of 8
    malloc_size=round_up(size);
    prsize_tf("malloc, the size is %d\n",size);
    prsize_tf("malloc, the size is %d\n",malloc_size);
    
    //first time call,the head posize_ter is null
    if(head_posize_ter == NULL)
    {
        //Allocate Enough Space
        void *heap_end;
        heap_end = sbrk(0);
        sbrk(4*malloc_size);
        tail_posize_ter = sbrk(0);
        
        if(heap_end == NULL)
        return NULL;
        //Find the posize_ter to the new seg
        heap_end = heap_end + 4;
        
        
        //malloc requested malloc_size + header for user
        size_t* tmp_head;
        tmp_head = (size_t*) heap_end;
        prsize_tf("segfault herre\n");
        *tmp_head = malloc_size + 4 ;
        
        prsize_tf("segfault herre\n");
        //Mask the last bit to keep track of a "used" seg
        *tmp_head = *tmp_head | 1;
        
        //The returned posize_ter to user;
        void *return_posize_ter;
        void *user_ptr;
        user_ptr = (void*) tmp_head;
        user_ptr = user_ptr + 4;
        return_posize_ter = user_ptr;
        
        //Then go to next free-block
        void *find_free_head;
        find_free_head = user_ptr + malloc_size;
        
        //Assign the header of the new free block
        dict header;
        header.size = 4*malloc_size - malloc_size - 4;
        header.prev = NULL;
        header.next = NULL;

        
        dict* tmp_head_posize_ter;
        tmp_head_posize_ter = (dict*) find_free_head;
        
        *tmp_head_posize_ter = header;
        head_posize_ter = (void*) tmp_head_posize_ter;
        prsize_tf("return successfully\n");


        return return_posize_ter;
        
    }
    
    //if it is not null
    
    dict* current_head;
    current_head = head_posize_ter;
    
    //Trasverse the linked list
    size_t count=0;
    while(1)
    {	

    	
        size_t tmp_size;
        dict tmp_dict;
        tmp_dict = *current_head;
        tmp_size = tmp_dict.size;


        //&& tmp_size >= 32
        if((tmp_size >= malloc_size + 4 ) || current_head->next == NULL)
        {
            
            //The fragment is enough to malloc
            //posize_ter current_head now posize_ts to the head of the spot to malloc
            
            //find out the user_head
            void* find_user_head;
            find_user_head = (void*) current_head;
            
            //malloc requested malloc_size + header for user
            size_t* tmp_head;
            tmp_head = (size_t*) find_user_head;
            *tmp_head = malloc_size + 4;
            
            
            
            find_user_head = find_user_head + 4;
            void* return_posize_ter;
            return_posize_ter = find_user_head;
            
            
            //Keep track of memory remained
            size_t memory_left;
            memory_left = tmp_size - malloc_size - 4;

            

            
            
            if(tmp_size < malloc_size + 4 + 24 + 8)
            {
                

                if(tmp_dict.next!=NULL)
                	{
                		
                		
                		dict* find_next_head;
                		find_next_head = tmp_dict.next;
                		dict next_head;
                		next_head = *find_next_head;

                		dict* find_prev_head;
                		find_prev_head = tmp_dict.prev;

                		next_head.prev = find_prev_head;
                		*find_next_head = next_head;


                		if(find_prev_head != NULL)
                		{
                			dict prev_head;
                			prev_head = *find_prev_head;
                			prev_head.next = find_next_head;
                			*find_prev_head = prev_head;
                		}
                		else
                		{
                			head_posize_ter = find_next_head;
                		}

                		*tmp_head = tmp_size;
                		
                		
                		

                	}
                else
                {
                    //Allocate New memory
                    size_t new_allocated_memory;
                    new_allocated_memory = 512 * size;
                    sbrk(new_allocated_memory);
                    tail_posize_ter = sbrk(0);
                    
                    //find out the next head
                    void* find_next_head;
                    find_next_head = find_user_head;
                    find_next_head = find_next_head + malloc_size;
                    
                    //Now find_next_head posize_ts to the start posize_t of next availabe block
                    dict* header_posize_ter;
                    header_posize_ter = find_next_head;
                    
                    //The header of the just-created free-block
                    dict header;
                    header.size = tmp_dict.size + new_allocated_memory - malloc_size - 4;
                    header.prev = tmp_dict.prev; 
                    header.next = NULL;
                    *header_posize_ter = header;
                    
                    
                    //The header of the previous free block
                    dict previous_head;
                    dict* previous_head_posize_ter;
                    previous_head_posize_ter = tmp_dict.prev;
                    if(previous_head_posize_ter != NULL)
                    {
                        previous_head = *previous_head_posize_ter;
                        previous_head.next = header_posize_ter;
                        *previous_head_posize_ter = previous_head;
                    }
                    else
                    {
                        head_posize_ter = find_next_head;
                    }
                    
                    
                }
    

        		//Mask the last bit to keep track of a "used" seg

            	*tmp_head = *tmp_head | 1;
                return return_posize_ter;
            }
            else
            {

                //Find the next free block
                void* find_next_head;
                find_next_head = find_user_head;
                find_next_head = find_next_head + malloc_size;
                
                //Now find_next_head posize_ts to the start posize_t of next availabe block
                dict* header_posize_ter;
                header_posize_ter = find_next_head;
                
                //If current node is already the last node in the linked list
                if(tmp_dict.next == NULL)
                {
                    
                    //The header of the just-created free-block

                    dict header;
                    header.size = memory_left;
                    header.prev = tmp_dict.prev; //question?
                    header.next = NULL;
                    *header_posize_ter = header;
                    
                    //The header of the previous free block
                    dict previous_head;
                    dict* previous_head_posize_ter;
                    previous_head_posize_ter = tmp_dict.prev;
                    if(previous_head_posize_ter != NULL)
                    {
                        previous_head = *previous_head_posize_ter;
                        previous_head.size = previous_head.size;
                        previous_head.prev = previous_head.prev;
                        previous_head.next = header_posize_ter;
                        *previous_head_posize_ter = previous_head;
                    }
                    else
                    {
                        head_posize_ter = find_next_head;
                    }
                }
                //if it is not the last node
                else
                {
                    //The header of the just-created free-block
                    ////(" next posize_ter is not null\n");
                    dict header;
                    header.size = memory_left;
                    header.prev = tmp_dict.prev;
                    header.next = tmp_dict.next;
                    *header_posize_ter = header;
                    
                    //The header of the previous free block
                    dict previous_head;
                    dict* previous_head_posize_ter;
                    previous_head_posize_ter = tmp_dict.prev;

                    if(previous_head_posize_ter != NULL)
                    {
                    	//("this situation?\n");
                    previous_head = *previous_head_posize_ter;
                    previous_head.size = previous_head.size;
                    previous_head.prev = previous_head.prev;
                    previous_head.next = header_posize_ter;
                    *previous_head_posize_ter = previous_head;
                	}
                	else
    				{
    					head_posize_ter = find_next_head;
    					//("look at here\n");
    				}

                    
                    //The header of the next free block
                    dict next_head;
                    dict *next_head_posize_ter;
                    next_head_posize_ter = tmp_dict.next;
                    next_head = *next_head_posize_ter;
                    
                    next_head.prev = header_posize_ter;
                    *next_head_posize_ter = next_head;
                    
                    
                    
                }
                //("Head posize_ter is at %p\n",head_posize_ter );
          		//("Allocated posize_ter is at %p\n",return_posize_ter );
        		//("Allocated Size is %d\n\n", malloc_size);
        		
        		//Mask the last bit to keep track of a "used" seg
            	*tmp_head = *tmp_head | 1;
                return return_posize_ter;
            }
        }
        
        if(current_head->next == NULL)
        break;
        
        current_head = current_head->next;
    }
    ("this happens\n");
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
* * it still posize_ts to the same (now invalid) location, and not to the
* * null posize_ter.
* *
* * @param ptr
* *    Posize_ter to a memory block previously allocated with malloc(),
* *    calloc() or realloc() to be deallocated.  If a null posize_ter is
* *    passed as argument, no action occurs.
* */
void free(void *ptr)
{
	//getchar();

    //"If a null posize_ter is passed as argument, no action occurs."
    if (!ptr)
    return;

    void* h_ptr;
    h_ptr = ptr - 4;

    
       

    //Find next free block
    void* find_next;
    find_next = h_ptr;

    if( find_next < head_posize_ter)
    	find_next = head_posize_ter;

    size_t tmp_current_size;
        size_t* next_ptr;
        next_ptr = (size_t*) h_ptr;
        tmp_current_size = *next_ptr & ~1;

        //("current_head_posize_ter is %p\n",head_posize_ter);
    //("free posize_ter is at %p\n",ptr );
      //("Free Size is %d\n\n", tmp_current_size);
    size_t count=0;
    while(1)
    {

    	//("current address is %p\n",find_next); 
    	
        size_t tmp_current_size;
        size_t* next_ptr;
        next_ptr = (size_t*) find_next;
        tmp_current_size = *next_ptr & ~1;

        if(!(*next_ptr & 0x1))
        {
            break;
        }
        find_next = find_next + tmp_current_size;
        
    		//break;
    }	

    //Unmask the current posize_ter;
	size_t current_size;
	size_t *curr;
	curr = (size_t*) h_ptr;
	*curr = *curr & ~1;
	current_size = *curr;

    //next free head
    dict next_head;
    dict* next_head_posize_ter;
    next_head_posize_ter = (dict*) find_next;
    next_head = *next_head_posize_ter;

    //prev head
    dict* find_prev;
    find_prev = next_head.prev;

    //current head
    dict current_head;
    dict* current_head_ptr;
    current_head_ptr = (dict*) h_ptr;
    current_head.size = current_size;
    current_head.prev = find_prev;
    current_head.next = next_head_posize_ter;

    //next head
    next_head.prev = current_head_ptr;
    *next_head_posize_ter = next_head;


    //coalescene possible
    if(find_next - h_ptr == current_size)
    	{
    		current_head.size = current_size + next_head.size;
    		current_head.next = next_head.next;

    		
    		dict next_next_head;
    		dict* next_next_ptr;
    		next_next_ptr = next_head.next;
    		if(next_next_ptr!= NULL)
    		{
    		next_next_head = *next_next_ptr;
    		next_next_head.prev = current_head_ptr;
    		*next_next_ptr = next_next_head;
    		}
    	}
    *current_head_ptr = current_head;

    
    dict prev_head;
    if(find_prev != NULL)
    {
    	prev_head = *find_prev;
    	prev_head.next = (dict*) h_ptr;
    	

    	//coalescene
    	if(find_prev + prev_head.size == h_ptr)
    	{
    		prev_head.size = prev_head.size + current_head.size;
    		prev_head.next = current_head.next;

    		dict prev_prev_head;
    		dict* prev_prev_ptr;
    		prev_prev_ptr = prev_head.prev;
    		if(prev_prev_ptr!= NULL)
    		{
    			prev_prev_head = *prev_prev_ptr;
    			prev_prev_head.next = current_head_ptr;
    			*prev_prev_ptr = prev_prev_head;
    			
    		}
    		next_head.prev = find_prev;
    		*next_head_posize_ter = next_head;


    	}
    	*find_prev = prev_head;
    }
    else
    {
    	head_posize_ter =  h_ptr;
    }
    

}


/**
* * Reallocate memory block
* *
* * The size of the memory block posize_ted to by the ptr parameter is changed
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
* * a new block of size bytes and returning a posize_ter to the beginning of it.
* *
* * In case that the size is 0, the memory previously allocated in ptr is
* * deallocated as if a call to free was made, and a NULL posize_ter is returned.
* *
* * @param ptr
* *    Posize_ter to a memory block previously allocated with malloc(), calloc()
* *    or realloc() to be reallocated.
* *
* *    If this is NULL, a new block is allocated and a posize_ter to it is
* *    returned by the function.
* *
* * @param size
* *    New size for the memory block, in bytes.
* *
* *    If it is 0 and ptr posize_ts to an existing block of memory, the memory
* *    block posize_ted by ptr is deallocated and a NULL posize_ter is returned.
* *
* * @return
* *    A posize_ter to the reallocated memory block, which may be either the
* *    same as the ptr argument or a new location.
* *
* *    The type of this posize_ter is void*, which can be cast to the desired
* *    type of data posize_ter in order to be dereferenceable.
* *
* *    If the function failed to allocate the requested block of memory,
* *    a NULL posize_ter is returned, and the memory block posize_ted to by
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
    //  is deallocated as if a call to free() was made, and a NULL posize_ter
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
