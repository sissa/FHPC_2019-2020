#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ptiming.h>

#define ALOT 100000000
#define AFEW 50

int main(void)
{
  uint64_t          clk1, clk2;
  volatile uint64_t clk;
  struct timespec   time;

  init_cycles_counter();

  printf("\nOn this systems there are %g ticks per nsec\n\n", clock_info.ticks_per_nsec);

  printf("clock calibration:\n"
	 "\tticks per nsec                 : %g\n"
	 "\tclock granularity in ticks     : %llu\n"
	 "\tclock granularity in nsec      : %g\n"
	 "\tclock overhead in ticks        : %llu\n"
	 "\tclock overhead dir in ticks    : %llu\n"
	 "\tclock overhead in nsec         : %g\n\n",
	 clock_info.ticks_per_nsec,
	 (unsigned long long)clock_info.clock_granularity,
	 clock_info.clock_granularity_nsec,
	 (unsigned long long)clock_info.clock_overhead,
	 (unsigned long long)clock_info.clock_overhead_direct,
	 clock_info.clock_overhead_nsec);


  
  
  clk1 = read_cyc_counter();
  for ( unsigned int ii = ALOT; ii-- ; )
    clk = read_cyc_counter();
  clk2 = read_cyc_counter();

  printf("\t%-20s : takes %llu ticks -> %g seconds\n\n",
	 "read_cyc_counter :",
	 (unsigned long long)((clk2 - clk1)/ALOT), (double)(clk2-clk1)/clock_info.ticks_per_nsec/_NSECs_IN_SEC/ALOT);
  

  clk1 = read_cyc_counter();
  for ( unsigned int ii = ALOT; ii-- ; )
    clock_gettime(CLOCK_MONOTONIC, &time);
  clk2 = read_cyc_counter();

  printf("\t%-20s : takes %llu ticks -> %g seconds\n\n",
	 "clock_gettime()",
	 (unsigned long long)((clk2 - clk1)/ALOT), (double)(clk2-clk1)/clock_info.ticks_per_nsec/_NSECs_IN_SEC/ALOT);

  clock_getres(CLOCK_MONOTONIC, &time);
  printf("getres says your monotonic clock has a resolution of: %g\n\n", (double)time.tv_sec + (double)time.tv_nsec * 1e-9);


  printf("----------------------------------------\n");


  
  
  long double sum;
  unsigned long long int avg = 0;
  for ( int jj = 0; jj < 30; jj++ )
    {
      start_counter();
      
      sum = 0;
      for ( unsigned int ii = AFEW; ii--; )
	sum += (double)ii;
      
      avg += get_counter;
    }

  printf("cycle took %Lg cycles (%Lg CPE) using start_counter()-get_counter() [sum is %Lg]\n", (long double)avg/30, (long double)avg/30/AFEW, sum );

  avg = 0;
  for ( int jj = 0; jj < 30; jj++ )
    {
      start_counter();
      
      sum = 0;
      for ( unsigned int ii = AFEW; ii--; )
	sum += (double)ii;
      
      avg += get_counter_ovhd;
    }

  printf("cycle took %Lg cycles (%Lg CPE) using start_counter()-get_counter() when counter overhead is accounted for [sum is %Lg]\n", (long double)avg/30, (long double)avg/30/AFEW, sum );

  avg = 0;
  for ( int jj = 0; jj < 30; jj++ )
    {

      clk1 = read_cyc_counter();
      
      sum = 0;
      for ( unsigned int ii = AFEW; ii--; )
	sum += (double)ii;
      
      clk2 = read_cyc_counter();
      avg += clk2 - clk1;
    }
  printf("cycle took %Lg cycles (%Lg CPE) using read_cyc_counter() [sum is %Lg]\n", (long double)avg/30, (long double)avg/30/AFEW, sum );


  avg = 0;
  for ( int jj = 0; jj < 30; jj++ )
    {

      clk1 = read_cyc_counter();
      sum = 0;
      for ( unsigned int ii = AFEW; ii--; )
	sum += (double)ii;

      clk2 = read_cyc_counter();
      avg += (clk2-clk1) - clock_info.clock_overhead_direct;
    }

  printf("cycle took %Lg cycles (%Lg CPE) using read_cyc_counter when counter overhead is accounted for [%Lg]\n", (long double)avg / 30, (long double)avg/30/AFEW, sum );

  
  
  printf("----------------------------------------\n");

  
  return 0;
}
