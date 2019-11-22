
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
#include <omp.h>


//
// HINT: play a bit with this code and the
//       environmental variables:
//       OMP_NUM_THREADS=n_0, n_1, n_2, ...
//       OMP_NESTED=<true|false>
//       OMP_MAX_ACTIVE_LEVELS=n
//


int function( int, int );

int main( int argc, char **argv )
{
#pragma omp parallel
  {
    int myid = omp_get_thread_num();
    function( myid, 0 );
  }

  return 0;
}


int function( int id, int nested )
{

  printf("\tthread %2d in function\n", id );
  
#pragma omp parallel num_threads(2)
  {
    int myid = omp_get_thread_num();
    printf("\t\tthread %2d,%2d in function\n", id, myid );
  }
}
