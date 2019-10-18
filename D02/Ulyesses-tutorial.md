# Learn to use the Ulysses cluster

- how to login into Ulysses 
- how to use modules 
- how to compile in serial
- how to compile an MPI application 
- how to submit jobs 
 - interactive usage 
 - batch usage
 - selecting resources ( cores and nodes/ walltime)
- how to run an MPI application on Ulysses


## Ulisse Environment Setup
Login:
```
$ ssh sissausername@frontend2.hpc.sissa.it
```
## use module 
Software is encapsulated in modules, check avaliability and and load modules with:
```
$ module avail
$ module load
```
## Run Serial Code
```
$ gcc pi.c -o serial_pi
$ ./serial_pi 1000000
```
## Run Parallel code

### Interactive mode
Load mpi modules
```
$ module load openmpi
loading dependency gnu/4.9.2
loading dependency numactl/2.0.10
loading dependency hwloc/1.10.0
```
Compile in parallel:
```
$ mpicc mpi_pi.c -o parallel_pi
```
Login on a node in interactive mode:
```
qsub -l nodes=1:ppn=20 -I
```
with this line we require one node in interactive mode and ask for 20 procecessors per node.
```
[user@login1 solution]$ qsub -l nodes=1:ppn=20 -I
qsub: waiting for job 199513.master1 to start
qsub: job 199513.master1 ready

[user@cn01-20 ~]$
```
Load mpi module again.
```
$ module load openmpi
loading dependency gnu/4.9.2
loading dependency numactl/2.0.10
loading dependency hwloc/1.10.0
```
Run the program:
```
$ mpirun -np 4 parallel_pi 10000
```


### Non interactive

just submit jobs..

Reserved queue `reserved3`.
