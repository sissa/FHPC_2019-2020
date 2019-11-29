# `libptiming`: a small library to access the CPU's  cycle counter



Here I present a very small and handy library that allows you to access the cycles counter of your cpu.

### A. Build the library

That is, or at least should be, quite easy. You find the executable bash script `make_ptiming` ( if it is not executable, make it such ). Executing it, you will find two new files, 

- `liptiming.so`, that obviously is the library itself
- ``test_ptiming`, that is a test code of the same library

Executing the latter, you may verify that the library is working properly:

![image-20191127212936319](/home/luca/work/TEACHING/DSSC/FHPC_2019-2020/libptiming/image-20191127212936319.png)



### B. Install the library

You can install (i.e. place the `libptiming.so` and the `ptimin.h` files in meaningful places on your disc) the library whenever it is more suited for you.



As an advice, a potentially nice way is the following.

1. create a `$HOME/lib` and a `$HOME/include` directories;
2. move `liptiming.so` in `$HOME/lib$`
3. move `ptiming.h` in `$HOME/include$`

This way, whenever you need to use the library (or whatever else library you compile and install), it will be sufficient something like the following:

```bash
cc -o my_program my_program.c -I$HOME/include -L$HOME/lib -lptiming
```

Shall you do the same for all your manually-installed library, besides this simplicity, in case you have to migrate your custom environment on some new machine, everything you will need is to copy both directories  on the new location.



### C. Use the library

A basic usage of the library is like that:

```C
#include <ptiming.h>

int main ()
{
	... ;                      // your code here
    init_cycles_counter();     // initialise lib
    ... ;
    
    unsigned long long int timing = 0;
    for ( int rr = 0; rr < REPETITION; rr++)
    {
    	start_counter();
    	...;                       
        for ( N_ITERATIONS ) {} // intensive code here
    	...;
    	timing += get_counter_ovhd;
    }
    
    printf("intensive code took %Lg cycles (%Lg CPE)\n",
          (long double)timing / REPETITION,
          (long double)timing / REPETITION / N_ITERATIONS)
        

}
```

