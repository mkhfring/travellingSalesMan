#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAFE(x_) {if((x_) == NULL) printf("out of memory. %d\n", __LINE__);}

unsigned long long factorial(int n);
void initialize(int * city_ids, int * graphWeights, int size);
int main(int argc, char const *argv[])
{
    printf("Starting the function\n");
    
    int size = 4, *cost;
    unsigned long long fact = factorial(size-1);
    printf("Factorial (%d), %llu\n", (size-1), fact);
    int *city_ids = (int *)malloc(size * sizeof(int));
    int *graphWeights = (int *)malloc(size * sizeof(int) * size);

	initialize(city_ids, graphWeights, size);

}
void initialize(int * city_ids, int * graphWeights, int size) {
	for (int i = 0; i < size; i++) {
		city_ids[i] = i;
		for (int j = 0; j < size; j++) {
			if (i == j)
				graphWeights[i * size + j] = 0;
			else
				graphWeights[i * size + j] = 99;
		}
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size;) {
			int next = 1; // (rand() % 2) + 1;
			int road = rand() % 100 + 1;
			if (i == j) {
				j += next;
				continue;
			}
			graphWeights[i * size + j] = road;
			j += next;
		}
	}

	for (int i = size - 1; i >= 0; i--) {
		graphWeights[((i + 1) % size) * size + i] = 1;
	}
  for(int i=0; i<size; i++){
      for(int j=0; j<size; j++){
          printf("%d,\t", graphWeights[i * size + j]);
      }
      printf("\n");
  }
}
unsigned long long factorial(int n) {
	int c;
	unsigned long long result = 1;

	for (c = 1; c <= n; c++){
		result = result * c;
  }

	return result;
}