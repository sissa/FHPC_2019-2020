/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *  2019
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

#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "aliasing.h"

int    compare  ( const void *A, const void *B );
TYPE * allocate ( int N );
  
TYPE *A, *B, *C;

TYPE * allocate( int N )
{
  TYPE *M;

  M = (TYPE*) calloc ( N*3, sizeof(TYPE) );
  A = M;
  B = A + N;
  C = A + 2*N;
  return M;
}

int main ( int argc, char **argv )
{

  int N = N_def;
  int top;
  int type = 0;
  double timing[ITER] = {0};
  double sum[ITER] = {0};
  TYPE *M;
  
  struct timespec ts;
  clockid_t       id = CLOCK_PROCESS_CPUTIME_ID;


  // ---------------------------------------------------
  // initialization
  
  if ( argc > 1 )
    {
      type = atoi( *(argv+1) );
      if ( argc > 2 )
	N = atoi( *(argv+1) );
    }

  M = allocate( N );

  srand48(1010101);
  for ( int cc = 0; cc < ITER; cc++ )    
    {
      A[cc] = drand48();
      B[cc] = drand48();
      C[cc] = 0;
    }


  // ---------------------------------------------------
  // set-up

  loopfunc function;

  if ( type < 2 )
    {
      if( type == 0)
	{
	  function = add_float_array;
	  printf("aliasing won't be explicitly excluded\n");
	}
      else
	{
	  function = add_float_array_noaliasing;
	  printf("aliasing will be explicitly excluded\n");
	}

      for ( int cc = 0; cc < ITER; cc++ )
	{
	  double time_start = TCPU_TIME;
	  function( N, C, A, B );
	  double time_end = TCPU_TIME - time_start;
	  
	  timing[ cc ] = time_end;
	  
	  // just to let the compiler believe we are doing something with C
	  for ( int ii = 0; ii < N; ii++ )
	    sum[cc] += C[ii];
	  memset( C, 0, sizeof(TYPE)*N);
	}
    }
  else
      for ( int cc = 0; cc < ITER; cc++ )
	{
	  double time_start = TCPU_TIME;
	  add_float_array_noaliasing_withconst( N, C, A, B );
	  double time_end = TCPU_TIME - time_start;
	  
	  timing[ cc ] = time_end;
	  
	  // just to let the compiler believe we are doing something with C
	  for ( int ii = 0; ii < N; ii++ )
	    sum[cc] += C[ii];
	  memset( C, 0, sizeof(TYPE)*N);
	}

  qsort ( timing, ITER, sizeof(double), compare);
  top = ITER/TOP_FACTOR;
  if (top < 3)
    top = 3;

  double time_avg  = 0;
  double sigma_avg = 0;
  for ( int cc = 0; cc < top; cc++ )
    {
      time_avg  += timing[cc];
      sigma_avg += timing[cc]*timing[cc];
      printf("%g ", sum[cc]);
    }
  time_avg /= top;
  sigma_avg /= top;
  
  printf( "\n - averaging best %d of %d iterations - %g (sigma^2: %g)\n",
	  top, ITER,
	  time_avg, sigma_avg - time_avg*time_avg);
  
  free( M );
  
  return 0;
}


int compare ( const void *A, const void *B )
{
  int lt = (*(double*)A > *(double*)B);
  int st = (*(double*)A < *(double*)B);
  return (lt - st);
}
