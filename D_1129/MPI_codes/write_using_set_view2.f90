! Created by S. Cozzini and G.P. Brandino for the Course of Data Management @ MHPC
! Last Revision by R. Aversa: October 2018

PROGRAM write_using_set_view

 use mpi
!! include 'mpif.h'    
 integer :: ierr, i, myrank, thefile,nsize,filetype
 integer, parameter :: BUFSIZE=4
 integer, dimension(BUFSIZE) :: buf
 integer(kind=MPI_OFFSET_KIND) :: disp

 call MPI_INIT(ierr)
 call MPI_COMM_RANK(MPI_COMM_WORLD, myrank, ierr)
 call MPI_COMM_SIZE(MPI_COMM_WORLD, nsize, ierr)
 do i = 1, BUFSIZE
    buf(i) = myrank * 10 + i
 enddo

 call MPI_FILE_OPEN(MPI_COMM_WORLD, 'testfile2', &
                   MPI_MODE_WRONLY + MPI_MODE_CREATE, &
                   MPI_INFO_NULL, thefile, ierr)

 call MPI_TYPE_SIZE(MPI_INTEGER, intsize,ierr)
 disp = myrank * BUFSIZE * intsize

!!    define a different pattern by means of MPI_TYPE_VECTOR
!!    first parameter: number of global element
!!    second parameter: number of blocks
!!    third parameter:  stride between block
!!
call MPI_TYPE_VECTOR(bufsize/2,2,2*nsize,MPI_INTEGER,filetype,ierr)
call MPI_TYPE_COMMIT(filetype, ierr)

disp = (bufsize/2) * myrank * intsize
 write(*,*) "myid ",myrank," disp ", disp

 call MPI_FILE_SET_VIEW(thefile, disp, MPI_INTEGER, &
                       filetype, 'native', &
                       MPI_INFO_NULL, ierr)

 call MPI_FILE_WRITE(thefile, buf, BUFSIZE, MPI_INTEGER, &
                       MPI_STATUS_IGNORE, ierr)
 call MPI_FILE_CLOSE(thefile, ierr)

 call MPI_FINALIZE(ierr)

END PROGRAM write_using_set_view

