/* Created by S. Cozzini and G.P. Brandino for the Course of Data Management @ MHPC
 * Last Revision by R. Aversa: October 2018
 */

#include<stdio.h>
#include<stdlib.h>
#include "mpi.h"
int main(int argc, char **argv){
	
	int i, rank, size, offset, N=16 ;
	int* buf=(int*) malloc(N*sizeof(int));

	MPI_File fhw;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	for ( i=0;i<N;i++){
	  buf[i] = i ;
  	}
  
	offset = rank*(N/size)*sizeof(int);

	MPI_File_open(MPI_COMM_WORLD, "datafile", MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &fhw);
	printf("Rank: %d, Offset: %d\n", rank, offset);
	MPI_File_write_at(fhw, offset, buf, (N/size), MPI_INT, &status);
	free(buf);
	MPI_File_close(&fhw);
	MPI_Finalize();

	return 0;
}
