#include <stdio.h>
#include <stdlib.h>

int main()
{

	malloc(1);
	int *ptr = malloc(200000*sizeof(int));
	
	if (ptr == NULL)
	{
		printf("Memory failed to allocate!\n");
		return 1;
	}

	*ptr = 4;
	free(ptr);

	printf("Memory was allocated, used, and freed!\n");	
	return 0;
}
