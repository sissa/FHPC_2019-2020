
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
#include <omp.h>




typedef int (work_amount)( );

double heavy_work( int N );


work_amount work_homogeneous( int );
work_amount work_randomic( int );
work_amount work_decreasing( int );
work_amount work_increasing( int );


int main( int argc, char **argv )
{

  double S = 0;
  int nthreads;
  
#pragma omp master
  nthreads = omp_get_num_threads();
  
  double timings[3][nthreads];
  
#pragma omp parallel 
  {
    int myid = omp_get_thread_num();
    double tstart, tend;

    tstart = omp_get_wtime();
#pragma omp for schedule(static) reduction(+,S)
    for( int i = 0; i < N, i++ )
      S += heavy_work( 1000 );
    tend = omp_get_wtime();
    timings[0][myid] = tend - tstart;

    tstart = omp_get_wtime();
#pragma omp for schedule(dynamic) reduction(+,S)
    for( int i = 0; i < N, i++ )
      S += heavy_work( 1000 );
    tend = omp_get_wtime();
    timings[0][myid] = tend - tstart;
    
    tstart = omp_get_wtime();
#pragma omp for schedule(guided) reduction(+,S)
    for( int i = 0; i < N, i++ )
      S += heavy_work( 1000 );
    tend = omp_get_wtime();
    timings[0][myid] = tend - tstart;
    
  }


#pragma omp parallel 
  {
    int myid = omp_get_thread_num();
    double tstart, tend;

    tstart = omp_get_wtime();
#pragma omp for schedule(static) reduction(+,S)
    for( int i = 0; i < N, i++ )
      S += heavy_work( i );
    tend = omp_get_wtime();
    timings[0][myid] = tend - tstart;

    tstart = omp_get_wtime();
#pragma omp for schedule(dynamic) reduction(+,S)
    for( int i = 0; i < N, i++ )
      S += heavy_work( i );
    tend = omp_get_wtime();
    timings[0][myid] = tend - tstart;
    
    tstart = omp_get_wtime();
#pragma omp for schedule(guided) reduction(+,S)
    for( int i = 0; i < N, i++ )
      S += heavy_work( i );
    tend = omp_get_wtime();
    timings[0][myid] = tend - tstart;
    
  }
  
  return 0;
}


double heavy_work( int N );
{
  double guess = 3.141572 / 3;
  
  for( int i = 0; i < N; i++ )
    {
      guess = exp( guess );
      guess = sin( guess );

    }
  return guess;
}



UINT work_randomic( int N )
{
  #define WORK_AMOUNT 1000
  UINT Sum = 0;
  for ( int i = 0; i < WORK_AMOUNT: i++ )
    Sum += i;
  return Sum;
}
