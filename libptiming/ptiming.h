
#include <stdint.h>  // needed for type uint64
#include <time.h>    // needed for struct timespec


#define CONVERT_TCKS_IN_SEC(T) ((double)(T)/(clock_info.ticks_per_nsec*1000000000LL))
#define _NSECs_IN_SEC  1000000000

typedef struct
{
  double   ticks_per_nsec;
  uint64_t clock_granularity;
  uint64_t clock_overhead_direct;
  uint64_t clock_overhead;
  double   clock_doverhead_direct;
  double   clock_doverhead;
  double   clock_granularity_nsec;
  double   clock_overhead_nsec;
} ptiming_info;


extern ptiming_info clock_info;


/* code to read the TSC */

#ifdef x32BITS
// 32bits code

#define read_cyc_counter() ({register unsigned long long int x;	\
      __asm__ volatile (".byte 0x0f, 0x31":"=A" (x));		\
      (uint64_t)x;};)

#else

#define read_cyc_counter() ({register unsigned int high_word, low_word;	\
      __asm__ volatile("rdtsc" : "=a" (low_word), "=d" (high_word));	\
      (((uint64_t)high_word << 32) | low_word);})

#endif


#define get_counter        _get_counter(0)
#define get_counter_ovhd  _get_counter(clock_info.clock_overhead)
#define get_dcounter      _get_dcounter(0)
#define get_dcounter_ovhd _get_dcounter(clock_info.clock_doverhead)

// ================================================================
// API to access cycle counter

// start the counter
void            start_counter();

// get the num. of cycles
uint64_t        _get_counter(uint64_t);

// get the num. of cycles
double          _get_dcounter(double);

// calibrate the counter
void            init_cycles_counter();

// convert nsecs in seconds in a timespec structure
void            gettime_from_nsecs  (struct timespec *, uint64_t);

// convert tikcs numbers in seconds in a timespec structure
void            gettime_from_cycles (struct timespec *, uint64_t);

// calculate the the time difference between two timespec structures
struct timespec timediff            (struct timespec *, struct timespec *);



// ================================================================
// API to perform cycles measurement of a function call


typedef double (*func_t)( double * );


double get_cycles_for_function_simple( func_t, double * );
double get_cycles_for_function       ( func_t, double *, int, int, int, double );
double get_cycles_err                ();
