

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#include "mp1-functions.h"

/**
 * (Edit this function to print out the ten "Illinois" lines in mp1-functions.c in order.)
 */
int helper(char *key)
{
	printf("%s\n", key);

}
int main()
{

	char *s;
	char t[4];
	t[0]='k';
	t[1]='e';
	t[2]='y';
	t[3]='\0';
	strcpy(t,"key");
	*s=t;
	helper(&t);


	return 0;
}
