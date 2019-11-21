/* =============================================================================
 * This file is part of the exercises for the Lectures on 
 *   "Parallel COmputing and OpenMP Introduction"
 * given at 
 *   Scientific and High Performance Computing School 2019"
 *   @ Università di Trento
 *
 * contact: luca.tornatore@inaf.it
 *
 *     This is free software (*); you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 *     This code is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License 
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>
 *     
 *     (*) also, let me add, has nothing particularly precious in it; in
 *         other words, do what you want with it, with the hope it may
 *         be useful in some way
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>


#define N_default 100

#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec +	\
		  (double)ts.tv_nsec * 1e-9)



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

  // just give notice of what will happen and get the number of threads used
#ifndef _OPENMP
  printf("serial summation\n");
#else
#pragma omp parallel
  {
#pragma omp master
    {
      nthreads = omp_get_num_threads();
      printf("omp summation with %d threads\n", nthreads );
    }
  }
#endif

  // initialize the array
  srand48( time(NULL) );
  for ( int ii = 0; ii < N; ii++ )
    array[ii] = (double)ii;
    //array[ii] = drand48();



  /*  -----------------------------------------------------------------------------
   *   calculate
   *  -----------------------------------------------------------------------------
   */


  double S       = 0;                                       // this will store the summation
  double runtime = 0;                                       // this will be the runtime

  double tstart  = CPU_TIME;
  
#if !defined(_OPENMP)
  
  for ( int ii = 0; ii < N; ii++ )                          // well, you may notice this implementation
    S += array[ii];                                         // is particularly inefficient anyway

#else

  
#pragma omp parallel for 
    for ( int ii = 0; ii < N; ii++ )
#pragma omp atomic
      S += array[ii];

#endif

  double tend = CPU_TIME;


  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */

  printf("Sum is %g, process took %g of wall-clock time, <%g> sec of thread-time \n", S, tend - tstart, runtime/nthreads );
  
  free( array );
  return 0;
}
