#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "papi.h"

extern int TESTS_QUIET;

/* Declared in test_utils.c */
#define PCHECK(e) \
  if (e!=PAPI_OK)						\
    {printf("Problem in papi call, line %d\n",__LINE__); return 1;}

#define NEVENTS 3
#define NRUNS 200
#define L1WORDS 4096  // double capacity of L1
#define L2WORDS 32768 // double capacity of L2

int main(int argc, char **argv)
{
  int events[NEVENTS] =
    {
      PAPI_TOT_CYC,/* total cycles */
      PAPI_L1_DCM, /* stalls on L1 cache miss */
      PAPI_L2_DCM, /* stalls on L2 cache miss */
    };
  
  long_long values[NEVENTS];
  PAPI_event_info_t info, info1;
  const PAPI_hw_info_t *hwinfo = NULL;
  int retval,event_code, m,n, i,j,size, arraysize;
  double *array;
  
  retval = PAPI_library_init(PAPI_VER_CURRENT);
  if (retval != PAPI_VER_CURRENT)
    printf("wrong PAPI initialization: %d instead of %d\n", retval, PAPI_VER_CURRENT);
  
  {
    int i;
    for (i=0; i<NEVENTS; i++) {
      retval = PAPI_query_event(events[i]) ; PCHECK(retval);
    }
    
  }
  
  /* declare an array that is more than twice the L2 cache size */
  arraysize=2*L2WORDS;
  array = (double*) malloc(arraysize*sizeof(double));
  for (size=L1WORDS/8; size<arraysize; size+=L1WORDS/8)
    {
      //printf("Run: data set size=%d\n",size);
      
      /* clear the cache by dragging the whole array through it */
      for (n=0; n<arraysize; n++) array[n] = 0.;
      
      /* now load the data in the highest cache level that fits */
      for (n=0; n<size; n++) array[n] = 0.;
      
      retval = PAPI_start_counters(events,NEVENTS); PCHECK(retval);
      /* run the experiment */
      for (i=0; i<NRUNS; i++)
	{
	  for (j=0; j<size; j++) array[j] = 2.3*array[j]+1.2;
	}
      
      retval = PAPI_stop_counters(values,NEVENTS); PCHECK(retval);
      
      /* printf("size = %d\nTot cycles: %lld\n",size,values[0]); */
      /* printf("cycles per array loc: %9.5f\n",values[0]/(1.*NRUNS*size)); */
      /* printf("L1 misses:\t%lld\nfraction of L1 lines missed:\t%9.5f\n", */
      /* 	     values[1],values[1]/(size/8.)); */
      /* printf("L2 misses:\t%lld\nfraction of L2 lines missed:\t%9.5f\n", */
      /* 	     values[2],values[2]/(size/8.)); */


      printf("size: %d cycles: %lld cycles_per_loc: %9.5f L1miss: %lld L1miss_frac: %9.5f L2miss: %lld L2miss_frac: %9.5f\n",
	     size,
	     values[0], values[0]/(1.*NRUNS*size),
	     values[1], (double)values[1]/(NRUNS*size),
	     values[2], (double)values[2]/(NRUNS*size));
  }
  free(array);

  return 0;
}
