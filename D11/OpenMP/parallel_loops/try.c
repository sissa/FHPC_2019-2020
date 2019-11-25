#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>

int main()
{

  unsigned long long int S = 0;
  unsigned long long int m = 100;

#pragma omp parallel for reduction(+:S) reduction(min:m)
  for( int i = 10; i < 100; i++ )
    {
      S += i;
      m = (i < m)?i:m;
    }

  printf("Sum is %llu\nMin is %llu\n", S, m);
  
  return 0;
}
