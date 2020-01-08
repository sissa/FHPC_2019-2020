
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <omp.h>


#define N_default      1000000  // how long is the main array
#define max_default    100000   // the maximum argument to heavy_work_? functions
#define factor_default 5000     // the size of the chunk assigned to each
                                // section

#if defined(_OPENMP)
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec +	\
		     (double)myts.tv_nsec * 1e-9)

#else

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)
#endif

#if !defined(NTHREADS)    // threads for the first level of parallelism
                          // this value should be equal to the number_of_sockets-1
#define NTHREADS 3
#endif

#if !defined(NTHREADS2)   // threads for the second level of parallelism
			  // this value should be equal to the number of
			  // cores per socket
#define NTHREADS2 8
#endif


#if defined(DEBUG)
#define PRINTF(...) printf(__VA_ARGS__);
#else
#define PRINTF(...)
#endif
  
typedef unsigned int uint;
double heavy_work_0( uint );
double heavy_work_1( uint );
double heavy_work_2( uint );



int main( int argc, char **argv )
{

  int      N         = N_default;
  int      max_value = max_default;
  int      factor    = factor_default;
  int      nthreads  = 1;
  const struct timespec nanot = {0, 1000};
  
  struct  timespec ts, myts;

  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 )
    {
      N = atoi( *(argv+1) );
      if( argc > 2 )
	{
	  max_value = atoi( *(argv+2) );
	  if( argc > 3 )
	    factor = atoi( *(argv+3) );
	}
    }

  if( N / (factor*NTHREADS) < NTHREADS)
    {
      while( N / (factor*NTHREADS) < NTHREADS )
	factor *= 0.8;
      PRINTF("factor decreased to %d\n", factor);
    }

  srand48(1234321);
  double result = 0;

  int *array = (int*)malloc( N*sizeof(double) );
  
#if !defined(_OPENMP)
  
  printf("serial summation\n" );

  double tstart = CPU_TIME;
  
  // this mimic a stream of number you can not
  // initialize in parallel
  //
  for( int ii = 0; ii << N; ii++ )
    array[ii] = lrand48() % max_value;
  
  for( int ii = 0; ii < N; ii++ )
    result += heavy_work_0(array[ii]) + heavy_work_1(array[ii]) + heavy_work_2(array[ii]);
			   
#else
			   
  printf("omp summation\n" );

  double tstart = CPU_TIME;
			   
  omp_set_num_threads( NTHREADS+1 );
#pragma omp parallel
#pragma omp single
  nthreads= omp_get_num_threads();
  
#if defined(DEBUG)
  if ( nthreads < NTHREADS+1 )
    {
      printf("something odd happened: you did not get"
  	     " as many threads as requested (%d instead of %d)\n",
  	     nthreads, NTHREADS+1 );
    }
#endif
  
  // using the c11 feature, you may decleare this
  // array to be nthreads long; however, we want
  // to have a compile-time initialization because
  // its size is linked to the number of omp sections
  //
  int    semaphores[NTHREADS] = {0};

  // we impose a spread policy on the outer level
  //
#pragma omp parallel proc_bind(spread) shared(result) firstprivate(nthreads)
  {
    // this is private for each thread
    // in the outer parallel region
    //
    int nchunks = N / factor + (N % factor > 0);

    // inside each section then, we spawn a new region
    // requesting a close policy; the places should be
    // defined accordingly at the moment of running
    // the program
    //
#pragma omp sections reduction(+:result)
    {

      #pragma omp  section                 // this section initializes the data
      {
	int idx = 0;
	int last  = factor;
	int first = 0;

	PRINTF("%d chunks will be sent to threads, each having %d entries (except for the last)\n",
	       nchunks, factor );
	for( int jj = 0; jj < nchunks; jj++ )
	  {
	    last = (last >= N)?N:last;
	    for( int kk = first; kk < last; kk++, idx++ )
	      array[idx] = lrand48() % max_value;
	    semaphores[jj % NTHREADS] = first;
	    PRINTF("initializer thread %d initialized chunk %d from %d to %d and signalled %d in position %d\n",
		   omp_get_thread_num(), jj, first, last, semaphores[jj%NTHREADS], jj%NTHREADS);
	    first += factor;
	    last  += factor;
	  }
	for( int jj = 0; jj < NTHREADS; jj++ )
	  semaphores[jj] = -1;
      }

      #pragma omp  section
      {
	int    prev_val = 0;
	int    me       = 0;
	double myresult = 0;
	while( prev_val >= 0 )
	  {
	    while( semaphores[me] == prev_val )
	      nanosleep(&nanot, NULL);
	    if( (prev_val = semaphores[me]) > 0 )
	      {
		int first = prev_val;
		int last  = prev_val + factor;
		last = (last > N)?N:last;
#pragma omp parallel for num_threads(NTHREADS2) reduction(+:myresult)
		for( int jj = first; jj < last; jj++ )
		  myresult += heavy_work_0( array[jj] ) +
		    heavy_work_1( array[jj] ) +
		    heavy_work_2( array[jj] );
	      }
	  }
	result += myresult;	    
      }

      #pragma omp  section
      {
	int    prev_val = 0;
	int    me       = 1;
	double myresult = 0;
	while( prev_val >= 0 )
	  {
	    while( semaphores[me] == prev_val )
	      nanosleep(&nanot, NULL);
	    if( (prev_val = semaphores[me]) > 0 )
	      {
		int first = prev_val;
		int last  = prev_val + factor;
		last = (last > N)?N:last;
                #pragma omp parallel for num_threads(NTHREADS2) reduction(+:myresult)
		for( int jj = first; jj < last; jj++ )
		  myresult += heavy_work_0( array[jj] ) +
		    heavy_work_1( array[jj] ) +
		    heavy_work_2( array[jj] );
	      }
	  }
	result += myresult;	    
      }


    }
  }



#endif
  
  double tend = CPU_TIME;


  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */


printf("The result is %g, summation took %g of wall-clock time\n\n",
       result, tend - tstart );


return 0;
}



double heavy_work_0( uint N )
{
  double guess = 3.141572 / 3;
  
  for( int i = 0; i < N; i++ )
    {
      guess = exp( guess );
      guess = sin( guess );

    }

  return guess;
}

double heavy_work_1( uint N )
{
  double guess = 3.141572 / 3;

  for( int i = 0; i < N; i++ )
    {
      guess = log( guess );
      guess = exp( sqrt(guess)/guess );
    }

  return guess;
}

double heavy_work_2( uint N  )
{
  double guess = 3.141572 / 3;

  for( int i = 0; i < N; i++ )
    {
      guess = sqrt( guess );
      guess = exp( 1+1.0/guess );
    }

  return guess;
}
