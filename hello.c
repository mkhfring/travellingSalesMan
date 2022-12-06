#include <stdio.h>
#include <mpi.h> 

int main (int argc, char *argv [])
{
    int my_rank , comm_sz;
    MPI_Init (&argc , &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz);
    printf("Greetings from process %d/%d\ n",my_rank , comm_sz );
    MPI_Finalize ();
    return 0;
}

