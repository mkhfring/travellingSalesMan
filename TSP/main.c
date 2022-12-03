#include <stdio.h>
#include <stdlib.h>


void getNodes(int **graph, int size);
void getNearestNeighborTour(int **graph, int *finalOrder, int size);
int main(int argc, char const *argv[])
{
    int numberOfNodes = atoi(argv[1]);
    int finalOrder[numberOfNodes];
    printf("The problem size is %d\n", numberOfNodes);
    int **graph;
    graph = malloc(numberOfNodes * sizeof(*graph));
    for(int i=0; i<numberOfNodes; i++){
      graph[i] = malloc(numberOfNodes * sizeof(*graph[i]));

    }
    getNodes(graph, numberOfNodes);
    getNearestNeighborTour(graph, finalOrder, numberOfNodes);
    
    /* code */
    return 0;
}
void getNodes(int **graph, int size){
    srand(17);
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(i==j)graph[i][j] = 0;
            else{
                float x = ((float)rand()*10/(float)(RAND_MAX)) * 10.0;
                if((int)x == 0) x++;
                graph[i][j] = (int) x;
                graph[j][i] = (int) x;
            }
        }
    }
}
void getNearestNeighborTour(int **graph, int *finalOrder, int size){
        for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            printf("%d,\t", graph[i][j]);
        }
        printf("\n");
    }
    finalOrder[0] = 0;
    int tour[size+1];
    tour[0] = 0;
    int visited[size];
    for(int i=0; i<size; i++){
        visited[i] = 0;
    }
    visited[0] = 1;
    int traverseSize = 0;
    int currentNode = 0;
    for(int i=0; i<size; i++){
        int best = INT8_MAX;
        int bestIndex;
        for(int j=0; j<size; j++){
            if(currentNode == j) continue;
            if(graph[currentNode][j] < best && visited[j]==0){
                best = graph[currentNode][j];
                bestIndex = j;
            } 
        }
        visited[bestIndex] = 1;
        currentNode = bestIndex;
        traverseSize++;
        tour[traverseSize] = bestIndex;
    }
    tour[traverseSize] = 0;
    printf("\nThe tour is:\n");
    for(int i=0; i< size+1; i++){
        printf("%d,\t", tour[i]);
    }

}
