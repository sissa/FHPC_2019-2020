# How to install [ior](https://github.com/hpc/ior/releases) on the cluster

First of all, request and node with centos 7, then load the modules:
```console
$ qsub -I -q ...
$ module load netcdf-fortran-parallel/4.4.4/netcdf-parallel/4.6.2/pnetcdf/1.10.0/hdf5-parallel/1.8.21/openmpi/3.1.2/gnu/7.2.0
```

Get the most recent version of `ior` from its [GitHub releases](https://github.com/hpc/ior/releases).
Versions older than 3.2.0rc1 come without mdtest, that's why we want to build the latest version (at the moment of this writing it's the `3.2.0rc1`, if a new version has been released, use it):
```console
$ VER=3.2.0rc1
$ wget https://github.com/hpc/ior/releases/download/${VER}/ior-${VER}.tar.gz
$ tar -xf ior-${VER}.tar.gz && cd ior-${VER}/
```
 
Configure and build it (if building for HDF5 1.6, add `CFLAGS=-D H5_USE_16_API`); version `3.2.0rc1` tries to include a lustre header not available on `c3e` (an issue has been opened for [this bug](https://github.com/hpc/ior/issues/115)), so we remove it with sed and then we proceed:
```console
$ sed -i 's|ac_header in linux/lustre/lustre_user.h|ac_header in|' configure
$ ./configure --prefix=$HOME/software/ior/${VER}/ CC=$(which mpicc) --with-lustre --with-mpiio --with-ncmpi --with-hdf5 --with-posix   # CFLAGS=-D H5_USE_16_API

$ make -j4
$ make install
```

To perform a benchmark with mdtest:
```console
$ cd $HOME/software/ior/${VER}/bin/
$ $(which mpirun) -np 8 $PWD/mdtest -d <filesystem> -n 10 -i 200 -y -N 60
```
where <filesystem> can be `lustre`, `NFS` (your home), or a local directory (`/tmp`).

To get more options for mdtest, read the help: `mdtest -h`.

To perform a benchmark with ior:
```
$ $(which mpirun) -np 8 $HOME/software/ior/${VER}/bin/ior -s 1 -d 5 -w -r -i 1 -e -t 1m -b 1g -vv -k -B -a API -o /path/to/testfile -F
```
where the most important details are:
* `-t 1m` the size that ior will use to save files to disk;
* `-b 1g` the total size of the file written by each process;
* `-o /path/to/testfile` is the path to a file that will be produced (and automatically deleted after tests); the path is important, because you can use different filesystems on cosilt: a `lustre` FS if you use your directory `/lustre/MHPC18/$USER/`, a `NFS` FS (your home), e.g. `/u/MHPC18/$USER/`, or a directory on a local disk (not a network FS, so you don't need to pass through the network and communicate to a metadata server), such as `/tmp` or `/local_scratch`;
* `-F` will make each process write its own dedicated file (e.g. if you passed `-o /lustre/testfile` and you are running an 8 cores MPI process, it will produce 8 temporary files `/lustre/testfile.0000000X`, with X varying from 0 to 7);
* API can be `POSIX|DUMMY|HDF5|MPIIO|NCMPI|MMAP` (`NCMPI` is the NetCDF version, built with [`parallel-netcdf`](https://parallel-netcdf.github.io/)).

For more info: `ior --help`.
