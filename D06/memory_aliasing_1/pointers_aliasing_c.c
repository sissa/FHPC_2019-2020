/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *
 *     This is free software; you can redistribute it and/or modify
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
 */


#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#define ITER 30
#define TCPU_TIME (clock_gettime( id, &ts ), (double)ts.tv_sec +\
                   (double)ts.tv_nsec * 1e-9)

#define TYPE float

int compare          ( const void *A, const void *B );
void add_float_array ( int , float*, float*, float* );


TYPE *A, *B, *C;


int main ( void )
{

  int N = 1000000;
  int top;
  double timing[ITER];
  double time_avg = 0;
  double sigma_avg = 0;

  struct timespec ts;
  clockid_t       id = CLOCK_PROCESS_CPUTIME_ID;

  
  A = (TYPE*) malloc ( sizeof(TYPE) * N );
  B = (TYPE*) malloc ( sizeof(TYPE) * N );
  C = (TYPE*) malloc ( sizeof(TYPE) * N );


  
  // call the function with memory aliasing
  for ( int cc = 0; cc < ITER; cc++ )
    {
      double time_start = TCPU_TIME;
      add_float_array( N, A, B, C );
      double time = TCPU_TIME - time_start;

      timing[ cc ] = time;
    }


  qsort ( timing, ITER, sizeof(TYPE), compare);
  top = ITER/10;
  if (top < 3)
    top = 3;
  
  for ( int cc = 0; cc < top; cc++ )
    {
      time_avg  += timing[cc];
      sigma_avg += timing[cc]*timing[cc];
    }
  time_avg /= top;
  sigma_avg /= top;
  
  printf( "\n - averaging best %d of %d iterations - %g (sigma^2: %g)\n",
	  top, ITER,
	  time_avg, sigma_avg - time_avg*time_avg);
  
  free( C );
  free( B );
  free( A );
  
  return 0;
}



void add_float_array( int N, TYPE * restrict C, TYPE * restrict A, TYPE * restrict B)
{

  for ( int i = 0; i < N; i++ )
    C[ i ] = A[ i ] + B[ i ];
  
  return;
}

int compare ( const void *A, const void *B )
{
  return *(int*)A - *(int*)B;
}
