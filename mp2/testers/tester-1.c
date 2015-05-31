#include <stdio.h>
#include <stdlib.h>

int main()
{
	int size = 1024*1024*128;
	void *ptr = malloc(size);
	void *ptr1 = realloc(ptr, size/2);
	void *ptr2 = malloc(size/2);
	void *ptr3 = realloc(ptr1, size/4);
	void *ptr3 = malloc( size/4);

	free(ptr4);
	free(ptr3);
	free(ptr2);
	free(ptr1);

	
	if (ptr == NULL)
	{
		printf("Memory failed to allocate!\n");
		return 1;
	}

	free(ptr);

	printf("Memory was allocated, used, and freed!\n");	
	return 0;
}
