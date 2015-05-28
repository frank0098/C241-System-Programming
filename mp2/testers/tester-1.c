#include <stdio.h>
#include <stdlib.h>

int main()
{

	printf("this shit\n");
	int *ptr = malloc(20000*sizeof(int));
	
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
