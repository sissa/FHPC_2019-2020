#
# Makefile for testing parallel I/O using netCDF library
#

NPROC = 8

MPIFC = mpif90
#FCFLAGS = -O0 -g -ffpe-trap=zero,invalid,overflow,underflow -fbacktrace \
#  -fdump-core -Wall -pedantic -Wconversion -Wsurprising -Waliasing \
#  -Wampersand -Warray-bounds -Wcharacter-truncation -Wuninitialized \
#  -Wline-truncation -Wintrinsics-std -Wno-tabs -Wunderflow \
#  -Wunused-parameter -Wno-align-commons -fno-range-check \
#  -Wimplicit-interface -Wimplicit-procedure -Wintrinsics-std \
#  -Wintrinsic-shadow -fcheck=array-temps,bounds,do,mem,recursion \
#  `nf-config --fflags`
FCFLAGS = -O3 -mtune=native `nc-config --fflags`
NCLIB = `nc-config --flibs`

all:: read_distribute collect_and_write allread allwrite

read_distribute : read_distribute.F90
	$(MPIFC) $(FCFLAGS) -o $@ $< $(NCLIB)

collect_and_write : collect_and_write.F90
	$(MPIFC) $(FCFLAGS) -o $@ $< $(NCLIB)

allread : allread.F90
	$(MPIFC) $(FCFLAGS) -o $@ $< $(NCLIB)

allwrite : allwrite.F90
	$(MPIFC) $(FCFLAGS) -o $@ $< $(NCLIB)

clean:
	rm -f read_distribute collect_and_write allread allwrite tutorial.nc

tutorial: read_distribute collect_and_write allread allwrite
	mpirun -np $(NPROC) ./allwrite dimparam.in
	mpirun -np $(NPROC) ./allread tutorial.nc
	mpirun -np $(NPROC) ./collect_and_write dimparam.in
	mpirun -np $(NPROC) ./read_distribute tutorial.nc
