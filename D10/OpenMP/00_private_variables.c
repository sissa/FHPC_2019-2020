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
#include <omp.h>


#define N_default 100


int main( int argc, char **argv )
{

  int    i, N = N_default;
  double a[N], b[N], c[N];                                     // yes, they live in the stack

  register unsigned long long base_of_stack asm("rbp");
  register unsigned long long top_of_stack asm("rsp");

  printf( "\nmain thread notice:\n"
	  "base of stack is: %p\n"
	  "top of stack is : %p\n"
	  "&i is           : %p\n"
	  "   &i - rbp     : %td\n"
	  "   &i - rsp     : %td\n",
	  (void*)base_of_stack,
	  (void*)top_of_stack,
	  &i,
	  (void*)&i - (void*)base_of_stack,
	  (void*)&i - (void*)top_of_stack );

  /* printf( "a %p %td, b %p %td, c %p %td, N %p %td, argc %p %td, &argv %p %td\n", */
  /* 	  &a, (void*)&a - (void*)base_of_stack, */
  /* 	  &b, (void*)&b - (void*)base_of_stack, */
  /* 	  &c, (void*)&c - (void*)base_of_stack, */
  /* 	  &N, (void*)&N - (void*)base_of_stack, */
  /* 	  &argc, (void*)&argc - (void*)base_of_stack, */
  /* 	  &argv, (void*)&argv - (void*)base_of_stack ); */
  
  // just initialize the arrays
  for ( i = 0; i < N; i ++ )
    {
      a[i] = i;
      b[i] = i+1;
    }

  // just try who is i for each thread
#pragma omp parallel private(i)
  {
    int me = omp_get_thread_num();
    register unsigned long long my_base_of_stack asm("rbp");

    printf( "\tthread nr %d says: i address is %p (%td from main\'s i)\n"
	    "\t\t\tmy base of stack is %p ( %td from main\'s stack )\n",
	    me, &i, (void*)&i - (void*)base_of_stack,
	    (void*)my_base_of_stack, (void*)base_of_stack - (void*)my_base_of_stack );
  }

#pragma omp parallel for shared(a,b,c,N) private(i)
  for( i = 0; i < N; i++ )
    c[i] = a[i]*b[i];


  printf( "\n" );  
  return 0;
}
