# First Assigment  FHPC course 

### Due date : 23.59.04.11.2019

***version 1.0 ( released at 9.30 28.11.2019)***

#### Submission policy:  by email to cozzini@iom.cnr.it as zip file named as Surname.FirstName.zip

Size of the file: less than 2MB
Structure of the files once decompressed:

	report.pdf
	codes/sumNumbers_mpi.[f90,c,cc]
	codes/readme

Report.pdf  will contain a detailed report on all the sections  in this documents. 
	
I strongly advise you to copy/edit  the structure of this  document  (in markdown format) with your preferred editor and keep the same structure of the document on your final report. Please make use of plots, tables to facilitate reading/understanding by us. 

Directory codes should includes all the programs we requested to write. The programs should be complemented by a readme file that explain which software stack you used to compile and run all the programs you provided  and on which machine. 



##Section 0: (warm-up)

- Compute Theoretical Peak performance for your laptop/desktop
  - Identify the CPU                       

  - Identify the frequency       

  - Identify how many cores   

  ​            Suggestion for Linux User:  `cat /proc/cpuinfo`  gives you all the information you neeed.

  - Identify the number of floating point for cycle  
    
      This is is tricky: try to find out some info about the global performance looking here:
      - https://en.wikipedia.org/wiki/FLOPS
      - https://stackoverflow.com/questions/23588380/how-to-compute-the-single-precision-data-and-double-precision-data-peak-performa?rq=1
  - Write here the  final result  obtained: 
  

  |        | Your model | CPU  | Frequency | number  of core | Peak performance |
  | ------ | ---------- | ---- | --------- | --------------- | ---------------- |
  | Laptop |            |      |           |                 |                  |
  |        |            |      |           |                 |                  |

 

- Compute sustained Peak performance for your  cell-phone
  
  - Identify an app to run HPL, for instance:  https://play.google.com/store/apps/details?id=com.sqi.linpackbenchmark
  
  - Run&Tune it to get the best number you can 
  
    Suggestion: try to increase the size of the matrix
  
  - Extra Bonus:  Identify the CPU and calculate the Peak performance of the CPU  
  
  
  |            | Your model | sustained performance | size of matrix | Peak performance | memory |
  | ---------- | ---------- | --------------------- | -------------- | ---------------- | ------ |
  | SmartPhone |            |                       |                |                  |        |
  |            |            |                       |                |                  |        |



- Find out in which year  your cell phone/laptop  could have been in top1% of Top500 

  - Fill in the following table

    

  |            | Your model | performance | Top500 year | number 1 HPC system | number of processors (TOP500) |
  | ---------- | ---------- | ----------- | ----------- | ------------------- | ----------------------------- |
  | SmartPhone |            |             |             |                     |                               |
  | Laptop     |            |             |             |                     |                               |


​    
## Section  1: theoretical model

- devise a performance model for a simple parallel algorithm: sum of N numbers

  - Serial Algorithm : n-1 operations 

     $T_{serial}= N*T_{comp}$ 
    $T_{comp}$= *time to compute a floating point operation*

  - Parallel Algorithm : master-slave	

    - read N and distribute N to P-1  slaves ===>  $T_{read} + (P-1)\times T_{comm}$ 
      $T_{comm}$ = *time  each processor takes to communicate one message, i.e. latency..*
      $T_{read}$   = *time  master takes to read* 

    - N/P sum over each processors (including master)  ===> $T_{comp}/N$

    - Slaves send partial sum  ===>   $(P-1)T_{comm}$

    - Master performs  one final sum ===>  $T_{comp}$

      the final model:    $T_p=   T_{comp}\times (1+ n/P)  + T_{read} + 2(P-1)\times T_{comm}  $

- compute scalability curves for such algorithm and make some plots

  - assumptions:

    - $T_{comp} =2 \times 10^{-9}$
    - $T_{read}= 1 \times 10^{-4}$
    - $T_{comm}= 1 \times 10^{-6}$

    Play with some value of N and plot against P  ( with P ranging from 1 to 1000)

- Comment on them:

  - For which values of N do you see the algorithm scaling ? 

  - For which values of P does the algorithm produce the best results ?

  - Can you try to modify the algorithm sketched above to increase its scalability ? 

    (hints: try to think a  better communication  algorithm) 

  

## Section 2 : play with MPI program

### 2.1:  compute strong scalability of a mpi_pi.c program

The application we are using here is a toy application: a Monte-Carlo integration to compute PI; The idea is to have a circle inscribed inside a square of unit lenght. The ratio between the area of the square (1) and the circle (pi/4) is $\pi/4$. Therefore, if we randomly choose N points inside the square, on average, only `M=N*pi/4` points will belong to the circle. From the last relation we can estimate pi. We provide a basic serial implementation of the algorithm ( program pi.c ) and we also give a parallel MPI implementation ( mpi_pi.c ) that computes PI by the same algorithm using MPI to compute the algorithm in parallel. Your exercise is to see how well this application scales up to the total number of cores of one node. You can modify the codes we have given, or you can write your own.

Steps to do:

- Compile the serial and parallel version.
- Determine the CPU time required to calculate PI with the serial calculation using 1000000 (10 millions) iterations (stone throws). Make sure that this is the actual run time and does not include any system time.

Hint: use /usr/bin/time command to time all the applications

- Get the MPI code running for the same number of iterations.

The parallel code writes walltime for all the processor involved. Which of these times do you want to consider to estimate the parallel time ?

- First let us do some running that constitutes a strong scaling test.

This means keeping the problem size constant, or in other words, keeping Niter = 10 millions. Start by running the MPI code with only one processor doing the numerical computation. A comparison of this to the serial calculationgives you some idea of the overhead associated with MPI. Again what time do you consider here ?

- Keeping Niter = 10 millions, run the MPI code for 2, 4, 8 and 16 and 20 MPI processs.

In principle, all that needs to be done is to run the program multiple times, changing the -np argument to mpirun each time. Such a study could be as simple as the following bash for loop:

```
 for procs in 1 2 4 8 16 32 64 ; do
 time mpirun -np ${procs} my-program the-input
 done
```

A real study is of course a litte bit more complicated because we neeed to get the performance of each of those runs. Furthermore we run on Ulysses (or any other HPC resources) and then we need to submit jobs.

In any case it is important to script your job executions, so that you can quickly measure scalability. So in this part of the lab you need to implement a strong scaling script. Script will be implemented by means of bash scripting, or if you prefer you can use python as well.

- Make a plot of run time  versus number of nodes from the data you have collected.
- Strong scalability here would yield a straight line graph. Comment on your results. Repeat the work playing with a large dataset.
- Provide a final plot with at least 3 different size and for each of the size report and comment your final results.

### 2.2: identify a model for the parallel overhead

In this section you are asked to identify a simple model that estimate the parallel overhead of our simple program as a function of the number of processor. We ask here to extend the simple model discussed previously and adapt/modify to the present program.

To do this please review carefully all the data collected so far and identify which is the indirect method to measure the parallel overhead from the output of the program.


### 2.3: weak scaling 

- Now let us do some running that constitutes a weak scaling test.

This means increasing the problem size simultaneously with the number of nodes being used. In the present case, increasing the number of iterations, Niter.
- Record the run time for each number of nodes and make a plot of the run time versus number of computing nodes.

- Weak scaling would imply that the runtime remains constant as the problem size and the number of compute nodes increase in proportion. Modify your scripts to rapidly collect numbers for the weak scalability tests for differrent number of moves.

- Plot on the same graph the efficiency (T(1)/T(p)) of weak scalability for different number of moves and comment the results obtained.

  

## Section 3: implement a parallel program using MPI 

We request here to implement the sum of N number using MPI paradigm. Programming language can be any among python C/ C++ and Fortran90.

The program should: 

- read from an input file  a number N arbitrarly large  (at least 1Billion number)

- distribute to each processor the amount of work to be done

- deals with cases where N (size of the array) is not exactly divisible by P (number of Processors) 

- collect the partial sums on one single processor and print it down.

- measure the reading time , the communication times (initial distribution phase and final collectin phase)  and the computation time (hints: use MPI_walltime)

- write as final output the time taken by each processor

  

## Section 4  (run and compare)

In this final section you are ruquested to run and measure strong scalability of  the program written in section 3.

  - Run the implemented program for different large enough sizes of the array 

  - Plot as in section 3 scalability of the program  

  - compare  performance results obtained against the performance model elaborated in section 2

      - comment on the assumption made in section 1 about times: are they correct ? do you observe something different ? 

        


