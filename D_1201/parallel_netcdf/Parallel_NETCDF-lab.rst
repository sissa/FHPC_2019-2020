Lab on Parallel netCDF
^^^^^^^^^^^^^^^^^^^^^^

1. Examine the provided Fortran Source code
2. How do the different strategies implemented work?
3. Analize performances of the different strategies with and without parallel FS
 + Change the grid size (try to increase in one or both dimensions)
 + Change the number of processors
4. (OPTIONAL) Modify the programs to read/write a 3D dataset. What is now the best
   strategy to read/write a datset using the netCDF library?
5. (OPTIONAL) Modify the 3D dataset reading programs to read a "vertical slice" along the
   third dimension in the central point of the processor domain. Examine the
   I/O performances. Can a different chunking help?

The participants are welcome to rewrite the source code in a programming
language of choice to reach expected goals.
