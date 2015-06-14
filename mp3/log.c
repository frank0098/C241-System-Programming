/** @file log.c */
#include <stdlib.h>
#include <string.h>
#include "log.h"

/**
 * Initializes the log.
 *
 * You may assuem that:
 * - This function will only be called once per instance of log_t.
 * - This function will be the first function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *   Pointer to the log data structure to be initialized.
 */
void log_init(log_t* l)
{
	for(int i=0;i<MAXSIZE;i++)
	{
		l->pointer[i]=NULL;
	}
	l->size = 0;
}

/**
 * Frees all internal memory associated with the log.
 *
 * You may assume that:
 * - This function will be called once per instance of log_t.
 * - This funciton will be the last function called per instance of log_t.
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure to be destoryed.
 */
void log_destroy(log_t* l)
{
	int curr_size = l->size;
	if(l->size !=0)
	{
		for(int i=0;i<curr_size;i++)
		{
			free(l->pointer[i]);
		}
	}
}

/**
 * Appends an item to the end of the log.
 *
 * The item MUST NOT be copied.  Only a pointer is stored in the log.
 *
 * You may assume that:
* - All pointers will be valid, non-NULL pointer.
*
 * @param l
 *    Pointer to the log data structure.
 * @param item
 *    Pointer to a string to be added to the log.
 */
void log_append(log_t* l, char *item)
{
	size_t item_len;
	item_len = strlen(item);
	char *new_ptr;
	new_ptr = malloc(item_len+1);
	strcpy(new_ptr,item);

	l->pointer[l->size] = new_ptr;
	l->size++;

}

/**
 * Removes and returns the last item in the log.
 *
 * If this function was called following a call to log_append(), the return
 * value will be the value that was just to the log.  If multiple calls are
 * made to log_pop(), is should continue to remove entries from the log in
 * a Last-In First-Out (LIFO) or "stack" order.
 *
 * You may assume that:
* - All pointers will be valid, non-NULL pointer.
*
 * @param l
 *    Pointer to the log data structure.
 *
 * @returns
 *    The last item in the log, or NULL if the log is empty.
 *
 */
char *log_pop(log_t* l)
{
    return NULL;
}

/**
 * Returns a pointer to the idx-th element in the log, where the
 * 0-th element is the first element appended to the log.
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 * @param idx
 *    Zero-based index into the log, where the 0-th entry corresponds to the
 *    first (oldest) entry in the log and the (n-1)-th entry corresponds to
 *    the latest (newest) entry in the log.
 *
 * @returns
 *    The idx-th entry in the log, or NULL if such an extry does not exist.
 */
char *log_at(log_t* l, unsigned int idx) 
{
    return 0;
}

/**
 * Returns the number of elements in the log.
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 *
 * @returns
 *    Number of entires in the log.
 */
unsigned int log_size(log_t* l)
{
    return 0;
}

/**
 * Preforms a newest-to-oldest search of log entries for an entry matching a
 * given prefix.
 *
 * This search starts with the newest or (n-1)-th entry in the log and
 * compares each entry to determine if query is a prefix of the log entry.
 * Upon reaching a match, a pointer to that element is returned.  If no match
 * is found, a NULL pointer is returned.
 *
 * For example, a log may be built with five entries:
 * @code
 *    log_append(&l, "ab  1");
 *    log_append(&l, "a   2");
 *    log_append(&l, "abc 3");
 *    log_append(&l, "ab  4");
 *    log_append(&l, "a   5");
 * @endcode
 *
 * Using the log that was build above:
 * - A call to <tt>log_search(&l, "ab")</tt> will return the pointer to the string "ab  4"
 *   since the search must begin from the newest entry and move backwards.
 * - A call to <tt>log_search(&l, "a")</tt> will return the pointer to the string "a   5".
 * - A call to <tt>log_search(&l, "abc")</tt> will return the pointer to the string "abc 3".
 * - A call to <tt>log_search(&l, "d")</tt> will return a NULL pointer.
 *
 * You may assume that:
 * - All pointers will be valid, non-NULL pointer.
 *
 * @param l
 *    Pointer to the log data structure.
 * @param prefix
 *    The prefix to test each entry in the log for a match.
 *
 * @returns
 *    The newest entry in the log whose string matches the specified prefix.
 *    If no strings has the specified prefix, NULL is returned.
 */
char *log_search(log_t* l, const char *prefix)
{
	int query_len = strlen(prefix);
	char comp_str[query_len];
	for(int i=l->size-1;i>=0;i--)
	{

		for(int j=0;j<query_len;j++)
			comp_str[j] = (l->pointer[i])[j];
		comp_str[query_len]='\0';
		if(strcmp(prefix,comp_str) == 0)
			return l->pointer[i];
		

	}
    return NULL;
}
