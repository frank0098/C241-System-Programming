#include <stdio.h>
#include <stdlib.h>

int main()
{
	int size = 1024;
	void *ptr = malloc(size);
	free(ptr);
	ptr = malloc(size);
	free(ptr);
	ptr = malloc(size);
	free(ptr);
	ptr = malloc(size);

	
	if (ptr == NULL)
	{
		printf("Memory failed to allocate!\n");
		return 1;
	}

	free(ptr);

	printf("Memory was allocated, used, and freed!\n");	
	return 0;
}
