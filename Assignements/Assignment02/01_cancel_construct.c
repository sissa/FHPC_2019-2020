
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ 2019-2020                                                                  │
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
#include <math.h>
#include <omp.h>


double do_some_work( int );

#define N_default 200000
#define WORKLOAD  100

int main( int argc, char **argv )
{

  int N   = N_default;
  int key = N / 2 + N/128;

  if ( argc > 1 )
    {
      key = atoi( *(argv + 1));
      if ( argc > 2 )
	N = atoi( *(argv+2));
    }

  #pragma omp parallel
  {
    int myid       = omp_get_thread_num();
    int exit_value = 0;

    // --------------------------------
    // this two vars are defined only to prove that
    // the loop-counter i is a different variable.
    int written = 0;
    int i;
    printf("thread %02d: i has address %p\n", myid, &i);
    // --------------------------------

    // --------------------------------
    // just to synchronize the output 
    // and put the double newline in the
    // right place
    fflush(stdout);
    #pragma omp barrier
    #pragma omp master
    printf("\n\n");
    #pragma omp barrier
    // --------------------------------
    
    #pragma omp for
    for ( i = 0; i < N; i++ )
      {
	exit_value = i;
	if ( written == 0) {
	  // very likely, the address of loop-counter i will be just 4 bytes
	  // after the address of the external i. That, however, is
	  // implementation-dependent.
	  printf("\tthread %02d: loop-counter i has address %p\n", myid, &i);
	  written = 1; }
	
	do_some_work( WORKLOAD );

	if ( i == key )
	  {
	    exit_value *= -1;
	    #pragma omp cancel for	     
	  }
        #pragma omp cancellation point for
      }

    // --------------------------------
    // just to synchronize the output
    // and put the double newline in the
    // right place
    fflush(stdout);
    #pragma omp barrier
    #pragma omp master
    printf("\n\n");
    // --------------------------------

    
    printf("%s thread %d exited at iteration %d (%p) \n",
	   (exit_value < 0?(exit_value*=-1,">>>> "):""), myid, exit_value, &i);
  }  
  
  return 0;
}



double do_some_work( int N )
{
  double guess = 3.141572 / 3 * N;
  
  for( int i = 0; i < N; i++ )
    {
      guess = exp( guess );
      guess = sin( guess );

    }
  return guess;
}
