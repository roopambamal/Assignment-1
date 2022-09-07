#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "crackme.h"

int main(int argc, char **argv){
	if (argc != 2){
		printf("Wrong usage: %s <size of password to generate>\n", argv[0]);
		exit(1);
	}
	int sizePass = atoi(argv[1]);
	
	int world_rank, world_size;
	MPI_Init(NULL, NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);

	if (!world_rank){
		printf("PID%d, %d\n", world_rank, p(sizePass, "ToTest0"));
	}
	else if (world_rank == 1){
		printf("PID%d, %d\n", world_rank, p(sizePass, "ToTest1"));
	}
	else if (world_rank == 2){
		printf("PID%d, %d\n", world_rank, p(sizePass, "ToTest1"));
	}
	else if (world_rank == 3){
		printf("PID%d, %d\n", world_rank, p(sizePass, "ToTest1"));
	}

	MPI_Finalize();
	return 0;
}