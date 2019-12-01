!
! Master in High Performance Computing
!
! MHPC - p1.6 Data Management
!
! Parallel reading of a netCDF file using netCDF library
!
! Author    : Graziano Giuliani <ggiulian@ictp.it>
! Institute : ICTP
! Date      : Fri Nov 21 09:24:42 CET 2014
!
program allread
  use netcdf
  use mpi
  implicit none
  integer :: icomm , cartcomm , irank , nproc
  real , allocatable , dimension(:,:) :: grid_local
  integer :: gnx , gny , lnx , lny , imiss
  integer :: li1 , li2
  integer :: lj1 , lj2
  integer :: ierr
  integer :: ncid , ivar , imode
  integer , dimension(2) :: idims
  integer , dimension(2) :: rdstart , rdstop
  integer :: n , nn
  integer , parameter :: iocpu = 0
  integer , dimension(2) :: cpus_per_dim
  integer , dimension(2) :: location
  integer , dimension(1) :: tmp
  integer , dimension(:) , allocatable :: istart , jstart , istop , jstop
  integer , dimension(:) , allocatable :: nix , njy , nptot
  logical , parameter , dimension(2) :: dim_period = (/.false., .false./)
  logical , parameter :: lreorder = .false.
  character(len=256) :: progname , infname
  real :: tstart , tfinish , ttot , tmean

  call get_command_argument(0, value=progname)
  call get_command_argument(1, value=infname)

  ! MPI initialization. Get communicator, rank and size
  call mpi_init(ierr)
  if ( ierr /= mpi_success ) then
    write(0,*) 'Cannot initialize MPI!'
    stop
  end if
  call mpi_comm_dup(MPI_COMM_WORLD, icomm, ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  call mpi_comm_rank(icomm, irank, ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  call mpi_comm_size(icomm, nproc, ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  nn = irank + 1

  ! If this is the iocpu , read input file and get dimensions

  imode = nf90_mpiio
  imode = ior(imode,nf90_nowrite)
  ierr = nf90_open(infname,imode,ncid,comm=icomm,info=mpi_info_null)
  call check_nc_error(ierr,__FILE__,__LINE__)

  ierr = nf90_inq_dimid(ncid,'x',idims(1))
  call check_nc_error(ierr,__FILE__,__LINE__)
  ierr = nf90_inquire_dimension(ncid,idims(1),len=gnx)
  call check_nc_error(ierr,__FILE__,__LINE__)
  ierr = nf90_inq_dimid(ncid,'y',idims(2))
  call check_nc_error(ierr,__FILE__,__LINE__)
  ierr = nf90_inquire_dimension(ncid,idims(2),len=gny)
  call check_nc_error(ierr,__FILE__,__LINE__)

  ! Require min 4x4
  if ( gnx < 4 .or. gny < 4 ) then
    call fatal(__FILE__,__LINE__, 'Minimum 4 points in each dimension needed')
  end if

  ierr = nf90_inq_varid(ncid,'globvar',ivar)
  call check_nc_error(ierr,__FILE__,__LINE__)

  ierr = nf90_var_par_access(ncid, ivar, nf90_independent)
  call check_nc_error(ierr,__FILE__,__LINE__)

  ! Logic for decomposition
  if ( nproc < 3 ) then
    cpus_per_dim(1) = nproc
    cpus_per_dim(2) = 1
  else
    if ( nproc == 4 ) then
      cpus_per_dim(1) = 2
      cpus_per_dim(2) = 2
    else
      cpus_per_dim(1) = (nint(sqrt(real(nproc)))/2)*2
      if ( gny > int(1.5*real(gnx)) ) then
        cpus_per_dim(1) = cpus_per_dim(1) - 1
        do while ( mod(nproc,cpus_per_dim(1)) /= 0 )
          cpus_per_dim(1) = cpus_per_dim(1) - 1
        end do
      else if ( gnx > int(1.5*real(gny)) ) then
        cpus_per_dim(1) = cpus_per_dim(1) + 1
        do while ( mod(nproc,cpus_per_dim(1)) /= 0 )
          cpus_per_dim(1) = cpus_per_dim(1) + 1
        end do
      else
        do while ( mod(nproc,cpus_per_dim(1)) /= 0 )
          cpus_per_dim(1) = cpus_per_dim(1) + 1
        end do
      end if
      cpus_per_dim(2) = nproc/cpus_per_dim(1)
    end if
  end if

  ! Create Cartesian topology
  call mpi_cart_create(icomm,2,cpus_per_dim,dim_period,lreorder,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)

  ! Get my position on Cartesian processor grid
  call mpi_cart_coords(cartcomm,irank,2,location,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)

  ! Logic for getting my local grid
  lnx = gnx/cpus_per_dim(1)
  lny = gny/cpus_per_dim(2)
  li1 = lnx * location(1)+1
  if ( lnx * cpus_per_dim(1) < gnx ) then
    imiss = gnx - lnx * cpus_per_dim(1)
    if ( location(1) < imiss ) then
      li1 = li1 + location(1)
      lnx = lnx + 1
    else
      li1 = li1 + imiss
    end if
  end if
  lj1 = lny * location(2)+1
  if ( lny * cpus_per_dim(2) < gny ) then
    imiss = gny - lny * cpus_per_dim(2)
    if ( location(2) < imiss ) then
      lj1 = lj1 + location(2)
      lny = lny + 1
    else
      lj1 = lj1 + imiss
    end if
  end if
  li2 = li1 + lnx - 1
  lj2 = lj1 + lny - 1

  allocate(istart(nproc), istop(nproc),  &
           jstart(nproc), jstop(nproc),  &
           nix(nproc), njy(nproc), nptot(nproc), stat=ierr)
  if ( ierr /= 0 ) call fatal(__FILE__,__LINE__, 'Memory allocation error !')

  tmp(1) = li1
  call mpi_allgather(tmp,1,mpi_integer4,istart,1,mpi_integer4,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  tmp(1) = li2
  call mpi_allgather(tmp,1,mpi_integer4,istop,1,mpi_integer4,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  tmp(1) = lj1
  call mpi_allgather(tmp,1,mpi_integer4,jstart,1,mpi_integer4,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  tmp(1) = lj2
  call mpi_allgather(tmp,1,mpi_integer4,jstop,1,mpi_integer4,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  tmp(1) = lnx
  call mpi_allgather(tmp,1,mpi_integer4,nix,1,mpi_integer4,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  tmp(1) = lny
  call mpi_allgather(tmp,1,mpi_integer4,njy,1,mpi_integer4,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  do n = 1 , nproc
    nptot(n) = nix(n)*njy(n)
  end do

  if ( irank == iocpu ) then
    write(6,*) 'Global grid dimension : ', gnx, ' x ', gny
    write(6,*) 'Number of processor in direction X : ', cpus_per_dim(1)
    write(6,*) 'Number of processor in direction Y : ', cpus_per_dim(2)
    do n = 1 , nproc
      write(6,'(a,i4,a,i5,a,i5,a,a,i5,a,i5,a,f9.3,a)')     &
        'Processor ',n,': [',istart(n),':',istop(n),'],',  &
                         '[',jstart(n),':',jstop(n),'] (', &
                         (float(nptot(n))/float(gnx*gny)*100.0),'%)'
    end do
  end if

  ! Allocate space for Local data
  allocate(grid_local(li1:li2,lj1:lj2), stat=ierr)
  if ( ierr /= 0 ) call fatal(__FILE__,__LINE__, 'Memory allocation error !')

  rdstart(1) = istart(nn)
  rdstart(2) = jstart(nn)
  rdstop(1) = nix(nn)
  rdstop(2) = njy(nn)

  call cpu_time(tstart)
  ierr = nf90_get_var(ncid,ivar,grid_local,rdstart,rdstop)
  call check_nc_error(ierr,__FILE__,__LINE__)

  call cpu_time(tfinish)
  ttot = tfinish - tstart
  call mpi_reduce(ttot,tmean,1,mpi_real,mpi_sum,iocpu,cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)
  if ( irank == iocpu ) then
    write(6,'(a,f6.3,a)') 'Mean elapsed time for the read operation : ', &
      tmean/real(nproc),' seconds.'
  end if

  ierr = nf90_close(ncid)
  call check_nc_error(ierr,__FILE__,__LINE__)

  call mpi_barrier(cartcomm,ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)

  write(6,*) 'Processor ',nn,' : ', maxval(grid_local), minval(grid_local)

  deallocate(istart,istop,jstart,jstop,nix,njy,nptot)
  deallocate(grid_local)

  call mpi_finalize(ierr)
  call check_mpi_error(ierr,__FILE__,__LINE__)

  contains

    subroutine check_nc_error(icode,cfile,iline)
      implicit none
      integer , intent(in) :: icode
      character(len=*) , intent(in) :: cfile
      integer , intent(in) :: iline
      if ( icode /= nf90_noerr ) then
        call fatal(cfile,iline,nf90_strerror(icode))
      end if
    end subroutine check_nc_error

    subroutine check_mpi_error(icode,cfile,iline)
      implicit none
      integer , intent(in) :: icode
      character(len=*) , intent(in) :: cfile
      integer , intent(in) :: iline
      if ( icode /= mpi_success ) then
        call fatal(cfile,iline,'MPI error !')
      end if
    end subroutine check_mpi_error

    subroutine fatal(cfile,iline,message)
      implicit none
      character(len=*) , intent(in) :: cfile , message
      integer , intent(in) :: iline
      character(len=8) :: cline
      write(cline,'(i8)') iline

      write(0,*) 'In file ',cfile,' at line ',adjustl(cline),' : ',message
      call mpi_abort(icomm, 1, ierr)
      if ( ierr /= mpi_success ) stop
    end subroutine fatal

end program allread

! vim: tabstop=8 expandtab shiftwidth=2 softtabstop=2
