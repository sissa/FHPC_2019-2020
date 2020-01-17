# How to use DARSHAN on C3E cluster 

## Load the module

Request a computing node and then:

```console
$ module load darshan/3.1.6/openmpi/3.1.2/gnu/7.2.0
```

It will load the latest module of Darshan available on the cluster (3.1.6)

Check whether the following command gives you any error:
```console
$ ldd $(echo $LD_LIBRARY_PATH |cut -d':' -f1)/libdarshan.so
```

If some error appears, like:
```
./libdarshan.so: /lib64/libc.so.6: version `GLIBC_2.14' not found (required by ./libdarshan.so)
```  
make sure to be on a CentOS 7 node (the master is CentOS 6).


### Compile your own darshan

The darshan module `darshan/3.1.6/openmpi/3.1.2/gnu/7.2.0` has been compiled with:
```console
$ module load openmpi/3.1.2/gnu/7.2.0
$ module load zlib/1.2.11/gnu/7.2.0

$ mkdir /dev/shm/darshan_$USER/  # do not compile on network-mounted FSs
$ wget ftp://ftp.mcs.anl.gov/pub/darshan/releases/darshan-3.1.6.tar.gz
$ tar -xf darshan-3.1.6.tar.gz && cd darshan-3.1.6/darshan-runtime/

$ ./configure --prefix=/opt/cluster/darshan/3.1.6/openmpi/3.1.2/gnu/7.2.0/ CC=$(which mpicc) --with-mem-align=8 --with-log-path-by-env=DARSHAN_LOG_DIR_PATH --with-jobid-env=PBS_JOBID
$ make -j4
$ make install
```

Note the `log-path-by-env` variable set to `DARSHAN_LOG_DIR_PATH`, you will need to set it to a directory of yours where darshan will place its logs.


##  Running with MPI

To run the code with darshan profiling library, write a script like this:

```bash
#!/bin/bash
#PBS -l walltime=1:00:00,nodes=2:ppn=24
#PBS -q NAME_OF_THE_QUEUE

module load openmpi/3.1.2/gnu/7.2.0
module load darshan/3.1.6/openmpi/3.1.2/gnu/7.2.0

export DARSHAN_LOG_DIR_PATH=$PBS_O_WORKDIR
MPIRUN=$(which mpirun)

$MPIRUN \
    -x LD_PRELOAD=$(echo $LD_LIBRARY_PATH |cut -d':' -f1)/libdarshan.so \
    -x DARSHAN_LOG_DIR_PATH \
        YOUR_MPI_APPLICATION
```
For example your application may be `ior -a MPIIO -wr -t1g -b1g -g -F -o $PBS_O_WORKDIR/testfile.ior`.


## Check results

After your application has completed, the darshan logs should be in the same directory you submitted the pbs script from, if you used the script above.

```console
$ ls -la
-r--------  1 exact     exact 19334 Dec  7 14:26 exact_ior_id58915_12-7-51888-16067085618417527621_1.darshan
-rw-rw-r--  1 exact     exact   793 Dec  7 14:24 ior_submit.sh
-rw-------  1 exact     exact    41 Dec  7 14:24 ior_submit.sh.e58915
-rw-------  1 exact     exact  2771 Dec  7 14:26 ior_submit.sh.o58915
```


## Use darshan tools to get information

## Install the analysis tools

Darshan is separated in two [modules](https://www.mcs.anl.gov/research/projects/darshan/documentation/): the `runtime` (installed and used above) and the `util`; the first is used to profile code, while the second is useful to build reports, and is usually installed on a local machine or on frontend nodes.

We provide an already built docker container with all the tools needed, because there are lots of latex dependencies needed in order to correctly create PDF reports; see the detailed instructions available [here](darshan-utils-docker/).


More information on the darshan utilities tools here:

http://www.mcs.anl.gov/research/projects/darshan/docs/darshan3-util.html#_analyzing_log_files

### Proposed exercise:

Play with darshan library and utilities on IOR and Mdtest  parallel benchmarks 


