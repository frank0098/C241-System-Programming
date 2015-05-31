#include <stdio.h>
#include <stdlib.h>

int main()
{
	int size = 1024;
	void *ptr = malloc(size);
	
	

	
	
	if (ptr == NULL)
	{
		printf("Memory failed to allocate!\n");
		return 1;
	}
	free(ptr);
	
	void *ptr2 = malloc(size);
	if (ptr2 == NULL)
	{
		printf("Memory failed to allocate!\n");
		return 1;
	}
	free(ptr2);
	

	printf("Memory was allocated, used, and freed!\n");	
	return 0;
}
