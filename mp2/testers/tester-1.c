#include <stdio.h>
#include <stdlib.h>

int main()
{
	int size = 1024;
	void *ptr = malloc(size);
	free(ptr);
	void *ptr2 = malloc(size);
	free(ptr);
	
	if (ptr == NULL)
	{
		printf("Memory failed to allocate!\n");
		return 1;
	}

	free(ptr2);

	printf("Memory was allocated, used, and freed!\n");	
	return 0;
}
