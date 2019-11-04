/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *   2019
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
#include "aliasing.h"


void assign_float_array( int N, TYPE *C, TYPE *A, TYPE *B)
{

  for ( int i = 0; i < N; i++ )
    {
      A[ i ] += C[ i ];
      B[ i ] += C[ i ];
    }
  
  return;
}


void assign_float_array_noaliasing( int N, TYPE * restrict C, TYPE * restrict A, TYPE * restrict B)
{

  for ( int i = 0; i < N; i++ )
    {      
      A[ i ] += C[ i ];
      B[ i ] += C[ i ];
    }
  
  return;
}


void assign_float_array_noaliasing_withconst( int N, const TYPE * restrict C, TYPE * restrict A, TYPE * restrict B)
{

  for ( int i = 0; i < N; i++ )
    {
      A[ i ] += C[ i ];
      B[ i ] += C[ i ];
    }
  
  return;
}
