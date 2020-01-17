# darshan practical on IOR

 In this exercise/tutorial we will use Darshan to profile and understand performance of parallel I/O approaches using the  IOR benchmark
 
 
 ## benchmark short description:
 
 IOR can be used for testing performance of parallel file
 systems using various interfaces and access patterns. IOR uses MPI for process synchronization
 
 ## exercise 1: 
 
 In this exercise we are profiling the I/O of some POSIX ways to read/write a single file or several files with Darshan.
 
Comparison between 48 tasks writing one single file vs. 48 tasks writing 48 different files
This exercise will help to check the difference between write/read a single file and write/read 1 file per task.

single share file: Command line to be used:

```
${EXE} -C -t 2m -b 500m -i 1 -a POSIX -w -r
Summary:
api = POSIX
test filename = testFile
access = single-shared-file
clients = 144 (72 per node)
repetitions = 1
xfersize = 2 MiB
blocksize = 500 MiB
aggregate filesize = 70.31 GiB
```

file-per process file: 

```
Command line used: ${EXE} -F -C -t 2m -b 500m -i 1 -a POSIX -w -r
Summary:
api = POSIX
test filename = testFile
access = file-per-process
clients = 144 (72 per node)
repetitions = 1
xfersize = 2 MiB
blocksize = 500 MiB
aggregate filesize = 70.31 GiB
```


HINT: To compare both summaries, we suggest you to use tkdiff command. Generate two different text files to compare redirecting stdout:

```
module load darshan
darshansummary user_xxxx_t2b500_IOR_xxx.darshan.gz > single-shared
darshansummary user_xxxx_t2b500F_IOR_xxx.darshan.gz > file-per-process
tkdiff single-shared file-per-process
```

What is the best way to achieve the best performance? Why?

## EXERCISE 2 Comparison of 48 tasks writing a single file using MPI-IO with and without stripe

In this exercise you are writing a single file of 70.31 GB in a folder that does not have
stripe and then in a folder with stripe. Both tests will use MPI-IO to write the file using
aligned chunks.

a. First you have to create two different folders called:
i. MPIIO
```
mkdir MPIIO
```
ii. MPIIO_stripe
```
mkdir MPIIO_stripe
```

b. Then set the 2M stripe to MPIIO-stripe. Use this command:
```
lfs setstripe -S 2097152 -c 4 MPIIO_stripe
```
This will set a stripe of 2MB per OST with a count of 4 OSTs per file.

Allowing MPI-IO to enhance the read/write Both arguments should be:
```
–C –t 2m –b 500m –I 1 –a MPIIO –w -r
```
