// C program to print all permutations with duplicates
// allowed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int count = 0;
 
/* Function to swap values at two pointers */
void swap(int* x, int* y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}
 
/* Function to print permutations of string
This function takes three parameters:
1. String
2. Starting index of the string
3. Ending index of the string. */
void permute(int* a, int l, int r, int* final)
{
    int i;
    if (l == r){
        printf("size %d\n", l);
        for(i=0; i<4; i++){
            printf("Inside %d \t", a[i]);
            final[i + count * (l+1)] = a[i];
        }
        printf("\n");
        count++;
    }

    else {
        for (i = l; i <= r; i++) {
            swap((a + l), (a + i));
            permute(a, l + 1, r, final);
            swap((a + l), (a + i)); // backtrack
        }
    }
}
 
/* Driver code */
int main()
{
    char str[] = "ABC";
    int *a = (int *)malloc(3 * sizeof(int));
    int *b = (int *)malloc(3 * 6 * sizeof(int));
    for(int i=0; i<4; i++){
        a[i] = i;
    }

    // int n = strlen(str);
    permute(a, 0, 4 - 1, b);
    for(int i=0; i<4*6; i++){
        printf("%d \t", b[i]);
    }
    return 0;
}