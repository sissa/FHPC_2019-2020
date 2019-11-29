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


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include "ptiming.h"
#define __USE_GNU
#include <sched.h>


/* =====================
 *  compile with:
 *    gcc -fPIC -march=native -O3 -c ptiming.c -o ptiming.o
 *    ld -shared ptiming.o -o libptiming.so
 * =====================*/

// library API
ptiming_info clock_info = {0, 0, 0, 0, 0, 0};


// internal use
const  uint64_t NSECs_IN_SEC = 1000000000LL;
uint64_t        counter_overhead = 0.0;
double          dcounter_overhead = 0.0;
static uint64_t last_cyc = 0.0;


// get the num. of cycles, minus the overhead
void            _get_counter_ovhd ( uint64_t *, double * );



void start_counter ()
/*
 * put cycles counters in static var
 */

{
  last_cyc = read_cyc_counter();
  return ;
}


uint64_t _get_counter ( uint64_t subtract_ovhd )
/*
 * access cycles counter and subtract
 * last call to counter
 */

{
  uint64_t local_cyc = read_cyc_counter();
  if ( local_cyc < last_cyc )
    return (__UINT64_MAX__ - last_cyc) + local_cyc - subtract_ovhd;
  else
    return local_cyc - last_cyc - subtract_ovhd;
  /* uint64_t counter_add = (local_cyc < last_cyc)? __UINT_MAX__ : 0; */
  /* return local_cyc + (counter_add - last_cyc); */
}


double _get_dcounter ( double subtract_ovhd )
{
  uint64_t local_cyc = read_cyc_counter();
  if ( local_cyc < last_cyc )
    return (double)((__UINT64_MAX__ - last_cyc) + local_cyc) - subtract_ovhd;
  else
    return (double)(local_cyc - last_cyc) - subtract_ovhd;
  
  /* uint64_t add       = (local_cyc < last_cyc)? __UINT_MAX__ : 0; */
  /* return (double)(local_cyc + counter_add - last_cyc);   */
}


// -----------------------------------------------------

// alternative version
void _get_counter_ovhd ( uint64_t *overhead, double *doverhead )
/*
 * access cycles counter and subtract
 * last call to counter
 */

{
#define N_SHOTS 30000
  
  uint64_t     result[N_SHOTS];
  long double dresult;
  for ( int i = 0; i < N_SHOTS; i++) {
    start_counter();
    _get_counter(0);
    result[i] = _get_counter(0);
  }
  dresult = (long double)(result[0]);
  for ( int i = 1; i < N_SHOTS; i++)
    {
      result[0] += result[i];
      dresult += (long double)result[i];
    }
  
  *overhead  = result[0] / N_SHOTS;
  *doverhead = dresult / N_SHOTS;
  return;
}





static void get_ticks_calibration();
  


/* this function calculates the time difference between two times*/
struct timespec timediff(struct timespec *T1, struct timespec *T2)
// assumes T1 is the end time, T2 is the begin time
{
  struct timespec temp;
  temp.tv_sec = T1->tv_sec - T2->tv_sec;
  temp.tv_nsec = T1->tv_nsec - T2->tv_nsec;
  
  if (temp.tv_nsec < 0) {
    temp.tv_sec--;
    temp.tv_nsec += NSECs_IN_SEC;
  }
  return temp;
}


static void get_ticks_calibration()
{
  struct timespec   time_begin, time_end, time_diff;
  uint64_t register clk_begin = 0, clk_end = 0;
  volatile double   accumulate = 0;


  // first calibration: gets how many ticks per seconds on this system
  // start time in seconds
  clock_gettime(CLOCK_MONOTONIC, &time_begin);
  // start time in cpu ticks
  clk_begin = read_cyc_counter();
  
  unsigned int i;
  for (i = 0; i < 1000000; i++)
    accumulate = (double)(i*i);

  // end time in cpu-ticks
  clk_end = read_cyc_counter();
  // end time in seconds
  clock_gettime(CLOCK_MONOTONIC, &time_end);

  // time difference in seconds
  time_diff = timediff(&time_end, &time_begin);

  // calibrate the ticks per seconds
  clock_info.ticks_per_nsec = (double)(clk_end - clk_begin)/(double)(time_diff.tv_sec * NSECs_IN_SEC + time_diff.tv_nsec);

  // second calibration: gets clock granularity

  
  
#define  N_CALIBRATION 100
  double timesfound[N_CALIBRATION];
  double ticks_granularity_threshold = 1./clock_info.ticks_per_nsec;
  
  for ( unsigned int i = 0; i < N_CALIBRATION; i++)
    {
      clk_begin = read_cyc_counter();
      while( (clk_end = read_cyc_counter() - clk_begin) < ticks_granularity_threshold)
	;
      timesfound[i] = clk_end;
    }
  
  double minDelta = ticks_granularity_threshold;
  double Delta;
  
  for(unsigned int i = 1; i < N_CALIBRATION; i++)
    {
      if ( timesfound[i] > 0 &&
	   timesfound[i] < minDelta )
	minDelta = timesfound[i];
      
      /* double Delta = timesfound[i] - timesfound[i-1]; */
      /* if( Delta != 0 )  // difference found in units of cycles */
      /* 	{ */
      /* 	  if(Delta < 0) */
      /* 	    Delta = -Delta; */
      /* 	  if( Delta < minDelta ) */
      /* 	    minDelta = Delta; */
      /* 	} */
    }

  uint64_t clock_granularity      = (minDelta < 1.0? 1.0 : minDelta);
  double   clock_granularity_nsec = CONVERT_TCKS_IN_SEC(minDelta) * 1e9; //1.0 / ticks_per_nsec; //
  
  clock_info.clock_granularity      = clock_granularity;
  clock_info.clock_granularity_nsec = clock_granularity_nsec;
  
  // third calibration: gets function call overhead
  
  clk_begin = read_cyc_counter();
  
  for ( unsigned int i = N_CALIBRATION*1000000; --i; ) {
    volatile uint64_t register temporary = read_cyc_counter();}
  
  clk_end = read_cyc_counter();
  
  uint64_t clock_overhead      = (uint64_t)((double)(clk_end - clk_begin) / (N_CALIBRATION*1000000));
  double   clock_overhead_nsec = CONVERT_TCKS_IN_SEC(clock_overhead);
  
  clock_info.clock_overhead_direct  = clock_overhead;
  clock_info.clock_doverhead_direct = (double)clock_overhead;
  clock_info.clock_overhead_nsec    = clock_overhead_nsec;

  _get_counter_ovhd ( &clock_info.clock_overhead, &clock_info.clock_doverhead );
  
}


void init_cycles_counter()
{
  unsigned long cpuMask;
  
  cpu_set_t set, set_save;
  size_t    size = sizeof(cpu_set_t);

  // save the current affinity mask
  sched_getaffinity(0, sizeof(cpu_set_t), &set_save);

  // clear the set
  CPU_ZERO(&set);
  
  // set affinity on core 0
  CPU_SET(0,&set);
  sched_setaffinity(0, sizeof(cpu_set_t), &set);

  // get how many ticks per nanoseconds and set ticks_per_nsec
  get_ticks_calibration();

  // set affinity to the initial mask
  sched_setaffinity(0, sizeof(cpu_set_t), &set_save);
}




void gettime_from_nsecs(struct timespec *ts, uint64_t nsecs)
{
  ts->tv_sec = nsecs / NSECs_IN_SEC;
  ts->tv_nsec = nsecs % NSECs_IN_SEC;
  return;
}


void gettime_from_tsc(struct timespec *ts, uint64_t tsc_count)
{
  uint64_t nsecs = tsc_count / clock_info.ticks_per_nsec;
  gettime_from_nsecs(ts, nsecs);
  return;
}



// ================================================================
// code to perform cycles measurement of a function call


static double       *samples     = NULL;
static unsigned int  maxnsamples = 0;
static unsigned int  nshots      = 0;

void   initialize_measures( int );
void   add_measure( double );
int    samples_have_converged( int, double );
void   clear_cache_content( void );
double get_cycles_err( int );


#define CLEARCACHE_DEFAULT   1
#define NCONVERGENCE_DEFAULT 5
#define NMEASURES_DEFAULT    500
#define EPSILON_DEFAULT      0.005
  
void initialize_measures( int n )
{
  if ( samples )
    free(samples);
  samples = (double*) calloc( n, sizeof(double) );
  maxnsamples = n;
  nshots      = 0;
}


void add_measure( double value )
{
  int idx;
  if ( nshots < maxnsamples )
    {
      idx = nshots;
      samples[ nshots ] = value;
    }
  else
    {
      idx = maxnsamples - 1;
      if ( samples[ idx ] > value )
	samples[ idx ] = value;
      else
	idx = -1;
    }
  
  nshots++;
  
  while ( (idx > 0) && (samples[idx-1] > samples[idx]) )
    {
      double tmp     = samples[idx-1];
      samples[idx-1] = samples[idx];
      samples[idx]   = tmp;
      idx--;
    }
    
  return;
}



int samples_have_converged( int which, double epsilon )
{
  if ( (nshots < which) || (maxnsamples <= which) )
    return -1;
  else
    {
      if ( (1+epsilon)*samples[0] >= samples[which] )	
	return 1;
      else
	return 0;
    }
}


double get_cycles_err( int which )
{
  if ( (nshots >= which) || (maxnsamples < which) )
    return (samples[which] - samples[0]) / samples[0];
  else
    return 10000.0;
}


void clear_cache_content()
{
  #define SINK_SIZE 4096
  double wash[ SINK_SIZE ];
  volatile double sink = 0;
  for ( int k = 0; k < 2; k++ )
    for ( int i = 0; i < SINK_SIZE; i++ )
      sink += wash[i];
  return;
}


double get_cycles_for_function ( func_t function, double *args, int clear_cache,
				 int n_for_convergence, int n_measures, double epsilon )
{

  initialize_measures( n_for_convergence );
  if ( clear_cache )
    clear_cache_content;

  do
    {
      function( args );
      uint64_t cyc_start = read_cyc_counter();
      function( args );
      uint64_t cyc_stop  = read_cyc_counter();
      double   cycles    = (double)( cyc_stop - cyc_start);
      add_measure( cycles );
    } while ( !samples_have_converged( n_for_convergence, epsilon) && nshots < n_measures );

  double result = samples[0];
  
  free( samples );
  samples = NULL;

  return result;
}




double get_cycles_for_function_simple ( func_t function, double *args )
{
  return get_cycles_for_function( function, args, CLEARCACHE_DEFAULT,
				  NCONVERGENCE_DEFAULT, NMEASURES_DEFAULT, EPSILON_DEFAULT );
}
