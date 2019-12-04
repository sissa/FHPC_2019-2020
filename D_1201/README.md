## Day 15: I/O storage system on HPC and parallel FS 



Date:  Monday  02/12/19

   

### *lectures given in "code-sharing"  with the  MHPC course in scientific data management* 

## OUTLINE

Lecture 5 gives a short overview of  storage system and associated concepts 

Lecture 6  gives on overview of the concept of parallel FS, present then the Lustre parallel Fs and its usage.


## SLIDES

- [Lecture 5 slides](L5-storage-and-FS.pdf)
- [Lecture 6 slides](L6-ParallelStorageandPFS.pdf)


## References&Books

- Mainly Chapter 17 and Chapter 18 of Sterling's book  
- http://www.prace-ri.eu/best-practice-guide-parallel-i-o/#id-1.3.5
- http://wiki.lustre.org/Main_Page and in particular the pdf below:
- http://wiki.lustre.org/images/6/64/LustreArchitecture-v4.pdf
- https://www.nics.tennessee.edu/computing-resources/file-systems/io-lustre-tips


## practical_sessions 

 - Identify your laptop FS and associated metadata
- [compile and run iozone](install_iozone.rst)
- measure I/O performance of your laptop by means of iozone
 - [practise with lustre FS](practise_with_lustre_FS.md)
 - [compile and run IOR + mdtest](install_IOR.md)
 - work in small groups to perform some benchmarks


## Benchmarks

1. Perform, by means of iozone, a benchmarking analysis on all the OSTs belonging to available lustre FS (C3HPC and Ulysses)
2. Perform, by means of IOR, a benchmarking analysis on different of different I/O approaches: POSIX/HDF5/MPI-IO 
3. Plot the results
4. Perform a detailed benchmarking comparative analysis of netcdf parallel codes (`allwrite` vs `collect_and_write`) with respect to:
   - size of the grid
   - number of processors
   - striping of the FILE
5. Plot the results

## additional materials

- IOR HPC benchmark [GitHub] page
- [IOR user guide]


[GitHub]: https://github.com/hpc/ior
[IOR user guide]: https://asc.llnl.gov/sequoia/benchmarks/IOR_User_Guide.pdf
[compile and run IOR + mdtest]: https://github.com/sissa/P2.3_seed/blob/master/lecture5-hpc-storage/install_IOR.md
[compile and run iozone]: https://github.com/sissa/P2.3_seed/blob/master/lecture5-hpc-storage/install_iozone.rst#how-to-install-and-run-iozone
[parallel netcdf exercise]: https://github.com/sissa/P2.3_seed/blob/master/lecture5-hpc-storage/parallel_netcdf/Parallel_NETCDF-lab.rst
[practise with lustre FS]: practise_with_lustre_FS.md
