

/*
 * PROPOSED EXERCISE
 *
 * Prefix Sum
 * --------------------------------
 *
 * Given an array A[ ], the prefix sum is an array B[ ] defined as
 *   B[i] = sum of the first i elements of A
 *
 * Alternatively, it can be done in-place on A, so that
 *
 *   A[i] = A[i] + sum of the first i-1 elements of A
 *
 * Here you find the naive code that perform that in serial.
 * Try to figure out how to parallelize it with OpenMP.
 *
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)

#define N_default 10000

int main( int argc, char **argv )
{

  int     N        = N_default;
  int     nthreads = 1;
  
  struct  timespec ts;
  double *array;

    /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 )
    N = atoi( *(argv+1) );


  // allocate memory
  if ( (array = (double*)calloc( N, sizeof(double) )) == NULL )
    {
      printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
      return 1;
    }

  
  // initialize the array

#ifdef _OPENMP
#pragma omp parallel
  {
    int me = omp_get_thread_num();
    unsigned int myt = time(NULL);
    unsigned short myseed[3] = { myt, myt+me, myt+me+me };
#pragma omp for
    for ( int ii = 0; ii < N; ii++ )
      array[ii] = erand48(myseed);
  }

#else

  srand48(time(NULL));
  for ( int ii = 0; ii < N; ii++ )
    array[ii] = drand48();
  
#endif


  /*  -----------------------------------------------------------------------------
   *   calculate
   *  -----------------------------------------------------------------------------
   */


  double tstart  = CPU_TIME;  
  
  for ( int ii = 1; ii < N; ii++ )
    array[ii] += array[ii-1];

  double tend = CPU_TIME;


  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */

  printf("process took %g of wall-clock time \n", tend - tstart);
  
  free( array );
  return 0;
}


