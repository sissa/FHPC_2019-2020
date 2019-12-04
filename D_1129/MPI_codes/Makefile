CC=mpicc
CFLAGS= -Wall
FC=mpif90
FFLAGS= -Wall

CSRC= MPI_SubArray.c
FSRC= write_using_set_view.f90
EXE= $(CSRC:.c=.x) $(FSRC:.f90=.x)

all : $(EXE)

%.x : %.c 
	$(CC) $(CFLAGS) $< -o $@

%.x : %.f90
	$(FC) $(FFLAGS) $< -o $@

clean:
	rm -rf *.x *.o
