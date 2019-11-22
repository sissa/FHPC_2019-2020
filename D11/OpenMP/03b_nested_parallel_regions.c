
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



int function( int, int );

int main( int argc, char **argv )
{
  int nested_is_active = omp_get_nested();
  int max_nest_level = omp_get_max_active_levels();
  int thread_limit   = omp_get_thread_limit();

  printf("nested parallelism is %s\n", (nested_is_active?"active":"disabled"));

  if ( nested_is_active )
    printf("maximum number of nested levels is %d\n", max_nest_level);

#pragma omp parallel
  {
    int myid = omp_get_thread_num();
    function( myid, 0 );
  }

  return 0;
}


int function( int id, int nested )
{
  int this_level = omp_get_active_level();
  int max_level  = omp_get_max_active_levels();
  int possible_parallelism = omp_get_max_threads();
  printf("\tthread %2d at level %2d/%2d spawning %d threads\n", id, omp_get_active_level(), omp_get_level(), possible_parallelism );

#pragma omp parallel if ( nested < max_level )
  {
    int myid = omp_get_thread_num();
    printf("\t\tthread %2d,%2d in function\n", id, myid );
    function( myid, this_level );
  }
}
