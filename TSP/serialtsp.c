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
void calculate_shortest_path(int *choices, int *weights, int size, unsigned long long fact, int *path);


int main(int argc, char const *argv[])
{
    printf("Starting the function\n");
    clock_t start, end;
    int size = 11, *cost;
    unsigned long long fact = factorial(size-1);
    printf("Factorial (%d), %llu\n", (size-1), fact);
    int *city_ids = (int *)malloc(size * sizeof(int));
    int *graphWeights = (int *)malloc(size * sizeof(int) * size);
    int *choices = (int *)malloc(size * sizeof(int) * fact);
    int *path = (int *)malloc(size * sizeof(int));
    start = clock();
	initialize(city_ids, graphWeights, size);
    //find_permutations(city_ids, choices, size, fact);
    permute(city_ids, 1, size-1, choices, size);
    calculate_shortest_path(choices, graphWeights, size, fact, path);
    end = clock();
    double time_passed = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("The execution time is %f\n", time_passed * 1000);
    printf("The shortest path is: \n");
    for(int i=0; i< size; i++){
        printf("%d, \t", path[i]);
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
        for(i=0; i<size; i++){
            final[i + permute_count * (l+1)] = a[i];
        }
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

void calculate_shortest_path(int *choices, int *weights, int size, unsigned long long fact, int *path){
    unsigned long long cost;
    unsigned long long min = INT64_MAX;
    int min_index;
    for(int i=0; i<fact; i++){
        cost = 0;
        for(int j=0; j<size; j++){
            cost += weights[j + i*size];
        }
        if(cost < min){
            min = cost;
            min_index = i;
        }
    }
    for(int i=0; i< size; i++){
        path[i] = choices[i + min_index * size];
    }
}