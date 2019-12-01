How to install and run iozone
=============================

Go to the iozone website and download the latest tarball::

  $ wget http://www.iozone.org/src/current/iozone3_471.tar
  $ tar -xvf iozone3_471.tar
  $ cd iozone3_471/src/current/
  
Now run make::

  $ make linux
  
Run the benchmark on Lustre, e.g.::

  $ cd /lustre/MHPC18/$USER/
  $ ./iozone -i 1 -i 0 -s 5g -r 1M -f ./5gzero2
  
An other example::

  $ mkdir ost0
  $ lfs setstripe -c 1 -i 0 ost0
  $ ./iozone -i 0 -i 1 -s 20m -r 1m -f ost0/poo
