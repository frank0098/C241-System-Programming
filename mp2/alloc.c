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
    int size;
    struct dict *prev;
    struct dict *next;
} *Dict;



void round_up(size_t size)
{
    size_t i;
    while(1)
    {
        if((size+i)%8==0)
        {
            return size+i;
            break;
        }
    }
}

Dict* head;

head = NULL;

void *malloc(size_t size)
{
    
    size_t malloc_size;
    
    malloc_size=round_up(size);		//malloc_size is the multiple of 8
    
    
    
    if(head == NULL)	//first time call,initialization
    {
        void *heap_end;
        void *user_head;
        void *return_head;
        
        Dict* tmp;
        heap_end = sbrk(16384);
        heap_end++;
        head = (Dict*) heap_end;
        head.size = size;
        head.prev = NULL;
        head.next = NULL;
        tmp++;
        user_head = void(*) tmp;
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
        
        Dict* next_head;
        next_head = (Dict*) user_head;
        next_head.size = 16384 - malloc_size;
        next_head.prev = head;
        next_head.next = NULL;
        head.next = next_head;
        
        return user_head;
        
    }
    
    Dict* org_head;
    org_head = head;
    while(org_head != NULL)
    {
        if(org_head.size >= malloc_size+12)
        {
            Dict* find_user_head;
            void* user_head;
            void* return_head;
            void* find_next_head;
            
            size_t memory_left;
            memory_left = org_head.size - malloc_size - 12; //Keep track of memory remained
            
            find_user_head = org_head;
            find_user_head++;
            user_head = (void*) find_user_head;
            return_head = user_head;
            
            if(org_head.size - malloc_size < 32)
            {
                
                return return_head;
            }
            else
            {
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
                    
                    Dict* next_head;
                    next_head = (Dict*) user_head;
                    if(org_head.next = NULL)
                    {
                        next_head.size = memory_left;
                        next_head.prev = org_head;
                        next_head.next = NULL
                        org_head.size = malloc_size;
                        org_head.next =  next_head;
                        
                    }
                    else
                    {
                        Dict* the_following_head;
                        the_following_head = org_head.next;
                        the_following_head.prev = next_head;
                        next_head.size = memory_left;
                        next_head.prev = org_head;
                        next_head.next = org_head.next;
                        org_head.size = malloc_size;
                        org_head.next =  next_head;
                        
                    }
                    
                    return return_head; 
                }  
            }  
        }

        org_head = org_head.next;
    }
    //if no proper spots, expand the heap
    //org_head.size < malloc_size+12

    Dict* find_last_head;
    find_last_head = head;
    while(find_last_head.next != NULL)	//need improvement?
    	find_last_head = find_last_head.next;

    sbrk(4*malloc_size);

    void* user_head;
    void* return_head;
    Dict* tmp_find_last_head;
    tmp_find_last_head = find_last_head;
    tmp_find_last_head++;
    Dict* user_head;
    user_head = (void*) tmp_find_last_head;
    return_head = user_head;
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

    Dict* next_head;
    
    find_last_head.size = 4*malloc_size + find_last_head.size;
    find_last_head.next = next_head;

    next_head = (Dict*) user_head;
    next_head.size = find_last_head.size - malloc_size;
    next_head.prev = find_last_head;
    next_head.next = NULL;

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
    // "If a null pointer is passed as argument, no action occurs."
    // if (!ptr)
    // 	return;
    
    // return;
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
