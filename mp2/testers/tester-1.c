#include <stdio.h>
#include <stdlib.h>

#ifdef PART2
  #define NUM_CYCLES 10000000
#else
  #define NUM_CYCLES 1
#endif

int main()
{
  void* p1 = malloc(300);
  void* p2 = malloc(300);
  void* p3 = malloc(300);
  free(p2);
  free(p3);
  void* p4 = malloc(400);
  void* p5 = malloc(100);
  printf("%p %p\n", p4, p5);
  int i;
  for (i = 0; i < NUM_CYCLES; i++) {
    int *ptr = malloc(sizeof(int));
    
    if (ptr == NULL)
    {
      printf("Memory failed to allocate!\n");
      return 1;
    }

    *ptr = 4;
    free(ptr);
  }

	printf("Memory was allocated, used, and freed!\n");	
	return 0;
}
