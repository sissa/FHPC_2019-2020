/* Created by S. Cozzini and G.P. Brandino for the Course P1.6 Data Management @ MHPC
 * Last Revision by R. Aversa: February 2018
 */

#include <stdlib.h>
#include <stdio.h>
#include "mpi.h" 
int main( int argc, char *argv[] ) 
{
    int 	 i,j; 
    int          gsizes[2], psizes[2], rank, size, m, n; 
    MPI_Datatype filetype; 
    int          local_array_size; 
    int          row_procs, col_procs; 
    int          dims[2], periods[2], lsizes[2], coords[2], start_indices[2]; 
    MPI_Comm     comm; 
    MPI_File     fh;  
    int          *local_array;  
    MPI_Status   status; 
 
    MPI_Init( &argc, &argv ); 
 
    /* This code is particular to a 2 x 3 process decomposition */ 
    MPI_Comm_size( MPI_COMM_WORLD, &size ); 
    if (size != 6) { 
	printf( "Communicator size must be 6\n" ); 
	MPI_Abort( MPI_COMM_WORLD, 1 ); 
    } 

    m=8;
    n=15; 
    row_procs = 2;
    col_procs = 3; 

    gsizes[0] = m;  /* no. of rows in global array */ 
    gsizes[1] = n;  /* no. of columns in global array*/ 
     
    psizes[0] = row_procs;  /* no. of processes in vertical dimension  
		       of process grid */ 
    psizes[1] = col_procs;  /* no. of processes in horizontal dimension  
		       of process grid */ 
 
    lsizes[0] = m/psizes[0];   /* no. of rows in local array */ 
    lsizes[1] = n/psizes[1];   /* no. of columns in local array */ 
 
    dims[0] = row_procs; 
    dims[1] = col_procs; 
    periods[0] = periods[1] = 0; 
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &comm); 
    MPI_Comm_rank(comm, &rank); 
    MPI_Cart_coords(comm, rank, 2, coords); 

    printf(" rank %d coords %d %d \n", rank, coords[0],coords[1]);
    /* See comments on block distribution */

    local_array = (int *)malloc( lsizes[0] * lsizes[1] * sizeof(int) );

    /* ... set elements of local_array ... */
    for (i=0;i<lsizes[0];i++)
	{
	for (j=0;j<lsizes[1];j++)
		{
		local_array[i*lsizes[1]+j]=rank*100+i*lsizes[1]+j;
		}
	} 
 
    /* global indices of the first element of the local array */ 
    start_indices[0] = coords[0] * lsizes[0]; 
    start_indices[1] = coords[1] * lsizes[1]; 
 
    MPI_Type_create_subarray(2, gsizes, lsizes, start_indices,  
                         MPI_ORDER_C, MPI_INT, &filetype); 
    MPI_Type_commit(&filetype); 
     
    MPI_File_open(MPI_COMM_WORLD, "datafile",  
		  MPI_MODE_CREATE | MPI_MODE_WRONLY,  
		  MPI_INFO_NULL, &fh); 
    MPI_File_set_view(fh, 0, MPI_FLOAT, filetype, "native",  
		      MPI_INFO_NULL); 
     
    local_array_size = lsizes[0] * lsizes[1]; 
    MPI_File_write_all(fh, local_array, local_array_size,  
		       MPI_INT, &status); 
     
    MPI_File_close(&fh); 
 
    MPI_Finalize(); 
    return 0; 
} 
