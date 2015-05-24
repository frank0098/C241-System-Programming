/** @file libdictioanry.c */

/*
* Machine Problem #1
* CS 241
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "libdictionary.h"

const int NO_KEY_EXISTS  = 1; /**< Return value if no key exists in the dictionary. @see dictionary_get() */
const int KEY_EXISTS     = 2; /**< Return value if a key exists in the dictionary. @see dictionary_add() */ 
const int ILLEGAL_FORMAT = 3; /**< Return value if the format of the input is illegal. @see dictionary_parse() */


/**
* Initializes the dictionary.  (If your data structure does not require any
* initialization logic, this function may be empty.)
*
* You may assume that:
* - This function will only be called once per dicitonary_t instance.
* - This function will be the first function called on each dictionary_t instance.
* - The dictionary pointer will be valid, non-NULL pointer.
*
* @param d
*   A pointer to the dictionary.  Since all values are passed by value in C,
*   you should never directly modify the parameter d.  For example:
*   @code
*     d = malloc( sizeof(dictionary_t) );
*     ...
*   @endcode
*   ...will not change d when d is returned to the caller.  Instead, you
*   should modify only the members of the structure that is pointed to by d,
*   such as:
*   @code
*     d->entry = malloc( sizeof(dictionary_entry_t) );
*     d->entry->value = ...
*     ...
*   @endcode
*/



void dictionary_init(dictionary_t *d)
{
    d->entry = NULL;
    //head = malloc(sizeof(dictionary_entry_t));
    //head = NULL;
    
}


/**
* Adds the key-value pair (key, value) to the dictionary, if and only if
* the dictionary does not already contain a key with the same name as key.
* This function does NOT make a copy of the key or value.  (You should NOT
* use strcpy() in the function at all.)
*
* You may assume that:
* - The stirngs key and value will not be modified outside of the dictionary.
* - The parameters will be valid, non-NULL pointers.
*
* @param d
*   A pointer to an initalized dictionary data structure.
* @param key
*   The key to be added to the dictionary
* @param value
*   The value to be assoicated with the key in the dictionary.
*
* @retval 0
*   Success
* @retval KEY_EXISTS
*   The dictionary already contains they specified key.
*/
int dictionary_add(dictionary_t *d, const char *key, const char *value)
{

    struct _dictionary_entry_t *curr;
    struct _dictionary_entry_t *new_entry;
    new_entry = malloc( sizeof(dictionary_entry_t) );

    char *tkey,*tvalue;
    tkey=malloc((strlen(key)+1)*sizeof(char));
    tvalue=malloc((strlen(value)+1)*sizeof(char));
    // tkey=malloc(100*sizeof(char));
    // tvalue=malloc(100*sizeof(char));
    strcpy(tkey,key);
    strcpy(tvalue,value);
    new_entry->key = tkey;
    new_entry->value= tvalue;
    new_entry->next = NULL;

    curr=d->entry;

    int flag=0;
    if(d->entry == NULL)
    {
        d->entry = new_entry;
        return 0;
    }
    else
    {

        while(1)
        {
            if(strcmp((curr->key),key)==0)
            {
                flag=1;
            }
            if(curr->next == NULL)
                break;

            curr=curr->next;

        }
        if(flag==1)
            {
                free(new_entry->key);
                free(new_entry->value);
                free(new_entry);
                return KEY_EXISTS;
            }
        else
        {
            curr->next = new_entry;
        }
        return 0;
    }
    
}


/**
* Parses the key_value string and add the parsed key and value to the
* dictionary. This function must make a call to dictionary_add() when
* adding to the dictionary.
*
* The format of the key_value will be the format of an HTTP header
* (@link http://www.w3.org/Protocols/HTTP/1.0/draft-ietf-http-spec.html#Message-Headers
* you can read more aboud the headers here, but this is not necessary to understand @endlink),
* where the contents will be the KEY (one or more non-colon characters), a colon,
* a single space, and the rest will be the VALUE. (While the KEY cannot be an empty
* string, it is possible for the VALUE to be an empty string.)
*
* This function should NOT copy key_value and should NOT create any additional memory to store
* the KEY and VALUE pieces of the string.  Instead, this function should modify the key_value
* string in-place, if necessary.  If the function fails, key_value should be unmodified.
*
* Valid inputs:
* - "Host: www.cs.uiuc.edu"
* - "MyKey: MyValue"
* - "a: b"
* - "SomeLongKey: ", where there is a trailing space after the colon
* - "Strange: but:this:is:okay: and: so: is: this"
* - "e: :e"
* - "This is not common: but it's not illegal"
* - "Strange:   case", where the value would be "  case"
*
* Illegal inputs:
* - ": www.cs.uiuc.edu", since the KEY is zero-length
* - "MyKey, MyValue", since no colon, space exists
* - "a:b", since no colon, space exists.
* - "a:: b", since the first colon isn't followed by a space.
*
* You may assume that:
* - All whitespace surrounding key_value has been removed.
* - The parameters will be valid, non-NULL pointers.
*
* @param d
*   A pointer to an initalized dictionary data structure.
* @param key_value
*   The key-value pair that is to be parsed and added to the dictionary.
*
* @retval 0
*   Success (the key_value was parsed and added to the dictionary)
* @reval KEY_EXISTS
*   The dictionary already contains a key with the same name as the KEY in key_value
* @retval ILLEGAL_FORMAT
*   The format of key_value is illegal.
*/
int dictionary_parse(dictionary_t *d, char *key_value)
{
    int len=strlen(key_value);
    int flag=0;
    int i;
    for ( i = 0; i < len; i++)
    {
        if(key_value[i]==':')
            {
                flag=1;
                break;
            }
    }
    
    if(flag==0 || i==0 || i==len)
        return ILLEGAL_FORMAT;
    char tkey[i+1];
    char tvalue[len-i+2];
    int j=0;
    for(j=0;j<i;j++)
    {
        tkey[j]=key_value[j];
    }
    tkey[j]='\0';
    for(j=0;j<len-i+2;j++)
    {
        tvalue[j]=key_value[i+j+2];

    }
    tvalue[j]='\0';

    dictionary_add(d,tkey,tvalue);
    return 0;


}


/**
* Returns the value of the key-value element for a specific key.
* If the key does not exist, this function returns NULL.
*
* You may assume that:
* - The parameters will be valid, non-NULL pointers.
*
* @param d
*   A pointer to an initalized dictionary data structure.
* @param key
*   The key to lookup in the dictionary.
*
* @return
*  - the value of the key-value element, if the key exists in the dictionary
*  - NULL, otherwise.
*/
const char *dictionary_get(dictionary_t *d, const char *key)
{
    struct _dictionary_entry_t *ptr;
    ptr=d->entry;
    
    while( ptr != NULL )
    {
    	if(strcmp((ptr->key),key)==0)
    	{
    		
    		return ptr->value;
    	}
    	else
    	{
    	 	ptr = ptr->next;
    
    	}
    }
    
    
    return NULL;
}


/**
* Removes the key-value pair for a given key from the dictionary, if it exists.
*
* You may assume that:
* - The parameters will be valid, non-NULL pointers.
*
* @param d
*   A pointer to an initalized dictionary data structure.
* @param key
*   The key to remove from the dictionary.
*
* @retval 0
*   Success.
* @retval NO_KEY_EXISTS
*   The dictionary did not contain key.
*/


int dictionary_remove(dictionary_t *d, const char *key)
{
    struct _dictionary_entry_t *ptr;
    struct _dictionary_entry_t *tmp;
    ptr=d->entry;
    tmp=d->entry;
    
    while( ptr != NULL )
    {
    	
    	if(strcmp((ptr->key),key)==0)
    	{
            //printf("remove %s\n",ptr->key);
    		if(ptr==d->entry)    //if it is the first element
            {
                tmp=ptr->next;
                d->entry=tmp;
                free(ptr->key);
                free(ptr->value);
                free(ptr);
                break;
            }
            else
            {
                tmp->next=ptr->next;
                free(ptr->key);
                free(ptr->value);
                free(ptr);
                break;
            }

    	}

    	tmp = ptr;
    	ptr = ptr->next;
    	 	
    
    }
    
    return -1;
}


/**
* Frees any memory associated with the dictionary.
*
* You may assume that:
* - This function will only be called once per dicitonary_t instance.
* - This function will be the last function called on each dictionary_t instance.
* - The dictionary pointer will be valid, non-NULL pointer.
*
* (Since <tt>d</tt> was provided for you by the programmer and not something you
*  created, you should NOT free <tt>d</tt> itself.  Only free member elements of
*  the data structure.)
*
* @param d
*   A pointer to an initalized dictionary data structure.
*/
void dictionary_destroy(dictionary_t *d)
{
    struct _dictionary_entry_t *tmp;
    tmp=d->entry;
    if(tmp == NULL)
        return;
    while(1)
    {
        tmp=d->entry;
        d->entry=tmp->next;
        free(tmp->value);
        free(tmp->key);
        free(tmp);
        if(d->entry==NULL)
            break;
            
    }


}
// int go(dictionary_t *d)
// {
//  struct _dictionary_entry_t *ptr;
//     ptr = d->entry;
    
//  while( ptr != NULL )
//     {
//      printf("what in? %s\n ",ptr->key);
//      ptr = ptr->next;
//     }
//    // printf("removenext\n");

// }