#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)


void F_000(double *R, int i)
{
  *R += i;
}

void F_001(double *R, int i)
{
  *R -= i;
}

void F_010(double *R, int i)
{
  *R *= i;
}

void F_011(double *R, int i)
{
  *R /= i;
}

void F_100(double *R, int i)
{
  *R += log10((double)i+1);
}

void F_101(double *R, int i)
{
  *R -= log10((double)i+1);
}

void F_110(double *R, int i)
{
  *R *= sin((double)i);
}

void F_111(double *R, int i)
{
  double di = (double)i;
  *R /= (sin(di) + cos(di));
}


#define mode_default  0
#define top_default   100000
#define case1_default 1
#define case2_default 1
#define case3_default 1


int main(int argc, char **argv)
{
  int mode  = mode_default;
  int top   = top_default;
  int case1 = case1_default;
  int case2 = case2_default;
  int case3 = case3_default;
  
  struct timespec ts;

  if (argc > 1)
    {
      mode  = atoi(*(argv+1));
      if ( argc > 4 )
	{
	  case1 = atoi(*(argv+2));
	  case2 = atoi(*(argv+3));
	  case3 = atoi(*(argv+4));

	  if ( argc > 5 )
	    top   = atoi(*(argv+5));
	}
    }
  

  double result = 1.0;
  int volatile i;

  if(mode == 0)
    {
      //      BLOCK A
      double start = CPU_TIME;
      
      for ( i = 1; i < top; i++)
        {
          if(case1 == 0)
            {
              if(case2 == 0)
                {
                  if(case3 == 0)
                    result += i;
                  else
                    result -= i;
                }
              else
                {
                  if(case3 == 0)
                    result *= i;
                  else
                    result /= i;
                }
            }
          else
            {
              if(case2 == 0)
                {
                  if(case3 == 0)
                    result += log10((double)i+1);
                  else
                    result -= log10((double)i+1);
                }
              else
                {
                  if(case3 == 0)
                    result *= sin((double)i);
                  else
                    result /= (sin((double)i) + cos((double)i));
                }
            }
              
        }
      double stop = CPU_TIME;
      printf("A :: elapsed seconds: %g\n", stop - start);
    }

  else
    {
      printf("no-branch case\n");
      //      BLOCK B

      void (*func)(double*, int);
      
      double start = CPU_TIME;
      if(case1 == 0)
        {
          if(case2 == 0)
            {
              if(case3 == 0)
                func = &F_000;
              else
                func = &F_001;
            }
          else
            {
              if(case3 == 0)
                func = &F_010;
              else
                func = &F_011;
            }
        }
      else
        {
          if(case2 == 0)
            {
              if(case3 == 0)
                func = &F_100;
              else
                func = &F_101;
            }
          else
            {
              if(case3 == 0)
                func = &F_110;
              else
                func = &F_111;
            }
        }
  
      for( i = 1; i < top; i++)
	{
	  double temp;
	  func(&temp, i);
	  result = temp;
	}

      double stop = CPU_TIME;
      printf("B :: elapsed seconds: %g\n", stop - start);

    }
      
  printf("result is : %g\n", result);

  return 0;
}
