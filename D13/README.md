# Exercises

Codes are available in the mpi_codes directory

## 1 - writeFile_offset.c and readFile_offset.c

1. Compile both codes. 
2. Check the `writeFile_offset.c` code and understand what it is
   doing.

   Select the correct answer, assuming the code runs on 4 processes:

   a. It writes 16 integers from 0 to 15

   b. It writes 16 integers, each process writes 0,1,2,3 

   c. It breaks your computer

3. Run the code using 4 processes. Compare your answer to the previous
   question with the output, by means of `od -iv` on the produced
   datafile.

4. Modify the code in order to print all the 16 integer by each
   process

5. Modify the code such that each process print a slice of the `buf`
   array.

6. Run the code using 1,2 and 3 processes. Is the code writing the 16
   integers in all cases?

7. Fix the code to run on an arbitrary number of processes, such that
   each process write a slice of the `buf` array.  (Hint: use the
   loadbalacing strategy )

8. Check the readFile_offset.c code and understand what it is doing.
  
   Answer the following questions:

   a. How many data each process reads?

   b. What do you expect each process to read?

9. Fix the code, to read the correct amount of data, as in point 5.
  
10. (Homework) Try to compile and run on different systems and see if
    the results are consistent.


## 2 - writeFile_pointer.f90 and readFile_pointer.c

1. Compile both codes.

2. Check the writeFile_pointer.f90 code and understand what it is
   doing.
 
   Select the correct answer:

   a. It does exactly what writeFile_offset does 

   b. It writes 16*[number of process] integers

   c. It breaks your computer

3. Run the code. Compare your answer to the previous question with the
   output, by means of `od -iv` on the produced datafile.

4. Modify the code such that it behaves as writeFile_offset as in 1-5

5. Run readFile_pointer and check that everything works correctly


## 4 - using set_view (Optional)

Code is available in the optional_code directory.

1. Take a look at the F90 code where `file_set_view` routine is introduced

2. Compile the code and compare results with output of writeFile_pointer.f90

3. Modify the code to use file_set_view using mpi derived data_type (MPI_TYPE_CONTIGUOUS or MPI_TYPE_VECTOR) and get the same results as the code you start from.

4. Modify the code to write the file using the following pattern (assuming you use 4 processes):

   1 2 11 12 21 22 31 32 3 4 13 14 23 24 33 34

   (hint: use MPI_TYPE_VECTOR)


## 5 - MPI_SubArray.c (Optional) 

Code is available in the optional_code directory.

1. Compile and run the MPI_SubArray.c code. The code is set now to run on a grid of 2x3 processes, and using a global matrix of 8x15. The code can
   just use a matrix such that the number of rows that is a multiple of the number of rows in the grid, and the number of columns that is multiple 
   of the number of columns in the grid.

   1. Check, by reading the code and by means of `od -iv`, what the code is doing.

2. Modify the code such it can handle an arbitrary matrix and an arbitrary grid.
   In the optional_code folder you can find a binary file called datafile_10x17_grid_3x5.
   This is the expected output for a matrix with 10 rows and 17 columns, on a process grid with
   3 rows and 5 columns. Your code should be able to produce this file, with the above parameters.


[file_methods]: https://mpi4py.scipy.org/docs/apiref/mpi4py.MPI.File-class.html
[file_IO]: http://pages.tacc.utexas.edu/~eijkhout/pcse/html/mpi-io.html
