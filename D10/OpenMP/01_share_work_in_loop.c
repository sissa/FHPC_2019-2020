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

int iters;
#pragma omp threadprivate(iters)




int main( int argc, char **argv )
{

  int     N        = N_default;
  int     nthreads = 1;

  // ---------------------------------------
  //  initialize
  
  // check whether some arg has been passed on
  if ( argc > 1 )
    N = atoi( *(argv+1) );

  // just give notice of what will happen and get the number of threads used
#pragma omp parallel
#pragma omp master
    {
      nthreads = omp_get_num_threads();
      printf("omp summation with %d threads\n", nthreads );
    }


    // ---------------------------------------
    //  calculate

    unsigned int S = 0;
  
#pragma  omp parallel for reduction(+:S)
    for ( int ii = 0; ii < N; ii++ )
      {
	iters++;
	S += (unsigned int)ii;
      }


    // ---------------------------------------
    //  finalize

#pragma omp parallel
#pragma omp critical
    printf( "\tthread %d performed %d iterations\n", omp_get_thread_num(), iters );
    
    printf("Sum is %u (expected: %d)\n", S, (N-1)*N/2 );
  

  return 0;
}
