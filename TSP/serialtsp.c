#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SAFE(x_) {if((x_) == NULL) printf("out of memory. %d\n", __LINE__);}
int permute_count = 0;

unsigned long long factorial(int n);
void initialize(int * city_ids, int * graphWeights, int size);
void find_permutations(int * city_ids, int * choices, int size, unsigned long long fact);
void reverse(int *first, int *last);
void swap(int *x, int *y);
void permute(int* a, int l, int r, int* final, int size);


int main(int argc, char const *argv[])
{
    printf("Starting the function\n");
    
    int size = 4, *cost;
    unsigned long long fact = factorial(size-1);
    printf("Factorial (%d), %llu\n", (size-1), fact);
    int *city_ids = (int *)malloc(size * sizeof(int));
    int *graphWeights = (int *)malloc(size * sizeof(int) * size);
    int *choices = (int *)malloc(size * sizeof(int) * fact);

	initialize(city_ids, graphWeights, size);
    //find_permutations(city_ids, choices, size, fact);
    permute(city_ids, 1, size-1, choices, size);
    for(int i=0; i<size*fact; i++){
        printf("choices %d \t", choices[i]);
    }

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

void swap(int* x, int* y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
void permute(int* a, int l, int r, int* final, int size)
{
    int i;
    if (l == r){
        printf("size %d\n", size);
        for(i=0; i<size; i++){
            printf("Inside %d \t", a[i]);
            final[i + permute_count * (l+1)] = a[i];
        }
        printf("\n");
        permute_count++;
    }

    else {
        for (i = l; i <= r; i++) {
            swap((a + l), (a + i));
            permute(a, l + 1, r, final, size);
            swap((a + l), (a + i)); 
        }
    }
}
void reverse(int *first, int *last) { while ((first != last) && (first != --last)) swap(first++, last); }
// void find_permutations(int * city_ids, int * choices, int size, unsigned long long fact) {
//     permute(city_ids, 0, size, choices, size);
//     for(int i=0; i<3*6; i++){
//         printf("%d \t", choices[i]);
//     }

	// int index = 1;
	// unsigned long long count = 0;
	// for (count = 0; count < fact; count++) {
	// 	for (int i = 0; i < size; i++) {
	// 		choices[i + count * size] = city_ids[i];
	// 	}
	// 	reverse(city_ids + index, city_ids + size);
	// }
    // for(int i=0; i<size * fact; i++){
    //     printf("choices %d", choices[i]);
    // }

//}