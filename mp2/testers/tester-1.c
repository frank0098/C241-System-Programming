#include <stdio.h>
#include <stdlib.h>

int main()
{
	int count=0;
	while(1)
	{
		count++;
		int *ptr = malloc(sizeof(int));
			if (ptr == NULL)
		{
			printf("Memory failed to allocate!\n");
			return 1;
		}

		//*ptr = 4;
		printf("the count is %d,the address is %p\n",count,ptr);
		free(ptr);

	}
	
	
	

	printf("Memory was allocated, used, and freed!\n");	
	return 0;
}
