! Created by S. Cozzini and G.P. Brandino for the Course of Data Management @ MHPC
! Last Revision by R. Aversa: October 2018

PROGRAM write_using_set_view

 use mpi
    
 integer :: ierr, i, myrank, thefile
 integer, parameter :: BUFSIZE=8
 integer, dimension(BUFSIZE) :: buf
 integer(kind=MPI_OFFSET_KIND) :: disp

 call MPI_INIT(ierr)
 call MPI_COMM_RANK(MPI_COMM_WORLD, myrank, ierr)
 do i = 1, BUFSIZE
    buf(i) = myrank * 10 + i
 enddo

 call MPI_FILE_OPEN(MPI_COMM_WORLD, 'testfile', &
                   MPI_MODE_WRONLY + MPI_MODE_CREATE, &
                   MPI_INFO_NULL, thefile, ierr)

 call MPI_TYPE_SIZE(MPI_INTEGER, intsize,ierr)
 disp = myrank * BUFSIZE * intsize

 call MPI_FILE_SET_VIEW(thefile, disp, MPI_INTEGER, &
                       MPI_INTEGER, 'native', &
                       MPI_INFO_NULL, ierr)

 call MPI_FILE_WRITE(thefile, buf, BUFSIZE, MPI_INTEGER, &
                       MPI_STATUS_IGNORE, ierr)
 call MPI_FILE_CLOSE(thefile, ierr)

 call MPI_FINALIZE(ierr)

END PROGRAM write_using_set_view

