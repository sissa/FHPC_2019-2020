! Created by S. Cozzini and G.P. Brandino for the Course of Data Management @ MHPC
! Last Revision by R. Aversa: October 2018

PROGRAM write_file
 USE MPI
 IMPLICIT NONE
 INTEGER :: err, i, myid, file, intsize
 INTEGER :: status(MPI_STATUS_SIZE)
 INTEGER, PARAMETER :: count=16
 INTEGER, DIMENSION(count) :: buf
 INTEGER(KIND=MPI_OFFSET_KIND) :: disp

 CALL MPI_INIT(err)
 CALL MPI_COMM_RANK(MPI_COMM_WORLD, myid,err)
 DO i = 1, count
   buf(i) = myid* count+ i
 END DO

  CALL MPI_FILE_OPEN(MPI_COMM_WORLD, 'datafile', MPI_MODE_WRONLY + MPI_MODE_CREATE, MPI_INFO_NULL, file, err)
  CALL MPI_TYPE_SIZE(MPI_INTEGER, intsize,err)
  disp= myid* count* intsize

  CALL MPI_FILE_SEEK(file, disp, MPI_SEEK_SET, err)
  CALL MPI_FILE_WRITE(file, buf, count, MPI_INTEGER, status, err)
  CALL MPI_FILE_CLOSE(file, err)

  CALL MPI_FINALIZE(err)
END PROGRAM write_file
