Scientific Data Formats
=======================

.. image:: template.png
   :height: 7.5cm
   :width: 14cm
   :alt: Scientific Data Formats
   :align: center

The File
--------

+ The **File** concept as a byte stream with a *single name* as the key to
  refer to a particular information, dates back to the origin of the
  Computer History.

+ In 1969, Thompson, R. H. Canaday, and Ritchie developed, on blackboards and
  scribbled notes, the basic design of a file system that was later to become
  the heart of Unix. 

From the beginning, the availability of storage, at the time tape storage,
required an API to save and retrieve data from magnetic support.

+ We still keep data on magnetic disk organized in hierarcical directories,
  with the name of the file as the first key of access.
+ Some OSes have built in indexing services that scan file content
  to build databases for metadata search engines

PDP-7
-----

In its PDP-7 implementation, the file system of PDP-7 Unix was nearly
identical to today's one.

+ An i-list: a linear array of i-nodes each describing a file.
  An i-node contained less than it does now, but the essential information
  was the same: the protection mode of the file, its type and size, and the
  list of physical blocks holding the contents. 
+ Directories: a special kind of file containing a sequence of names and the
  associated i-number. 
+ Special files describing devices. The device specification was not contained
  explicitly in the i-node, but was instead encoded in the number: specific
  i-numbers corresponded to specific files. 

The important file system calls were also present from the start.
Read, write, open, creat, close: they were similar to what one finds now.

Scientific Data
---------------

The File structure has been adapted to store Scientific Data. Now we have
better than tapes, but the File Sytem has not undergone drastic changes.

**We still have directories and file names as base.**

+ Is this satisfactory?
+ Can just a name, a single string (in most cases with OS imposed limitations),
  contain all relevant information on Scientific data?

For *Scientific Data*, we need to store also MetaData, and in the case of
big data, we need to slice and dice the datasets.

+ How to add all needed MetaData to the physical quantities?
+ How to structure multi-dimensional datasets in a file?

Atmospheric Science Case
------------------------

- The status of the atmosphere is obtained with measurements. Measure
  need to be exchanged through the network to provide all modeling sites with
  the needed information to have weather forecast.
  Station data, Upper Air Profiles (Soundings), Satellite Images are all
  collected and transfered. The BUFR (Binary Universal Form for data
  Representation) standard is 130+ pages, and describes basic tables to define
  data structure.

- For Model Output data, the WMO GRIB format - GRIdded Binary -
  requires 103 pages to describe the STANDARD meaning of look-up tables and
  describe the storage format, and NO reference implementation of the I/O
  API exist.

Multiple communities has started the search for a solution to have a single
API to store multiple data objects with strong relations into a single file
object. We will now have a look at **HDF5** and **netCDF** file formats, which
help us overcome the metadata and multi-dimensionality problen using a
standard.

HDF Solution
------------

HDF stands for Hierarchical Data Format. We store data in a single file
like objects of a hierarchical structure similar to a file system, but we
have standard methods to organize the data with relations. The API library
with almost ANY programming language interface allows our data to be portable.

The Software ecosystem built on top of HDF5 data allows the data to be
used in a number of data analysis/visualization/archiving/distribution
programs and platforms.

The latest implementation of the HDF file format is the HDF5, and can be
download from the HDFGROUP_ :

         http://www.hdfgroup.org

.. _HDFGROUP: http://www.hdfgroup.org

HDF Pros and Limitations
------------------------

HDF files have:

- Self-descriptive access
- Groups of data in a tree like structure
- Metadata as attributes to data structure
- Efficient storage mechanism for large numerical arrays
- Paralelle I/O capabilities on top of the MPI library.

HDF does not have:

- Ease of access for small 1D table like datasets
- STRONG relational databases capabilities

The HDF5 file specification and open source library is maitained by a nonprofit
US based organization. The Software implementation of the HDF5 is actively
maintained and developed, and has a commitment for backwards compatibility
to allow data archiving for long term storage.

HDF5 model
----------

The main pieces of the HDF5 model are:

- Datasets
- Groups
- Attributes

The User code access through the API this public abstractions, and has not to
bother about the implementation internal data structure or the low level
drivers to access the physical bytes on disk.

For example the HDF5 *core* driver allows the user to access in memory files,
the *family* driver let the user split a single dataset in multiple on-disk
files, the *mpio* driver allows the parallel I/O MPI library use to access the
data in the file.

We will use the simple H5PY_ Python interface from now on to illustrate the
basic concepts in a HDF5 file usage.

  http://www.h5py.org

.. _H5PY: http://www.h5py.org

Creating HDF5 file
------------------

The below creates an empty HDF5 file structure, and adds a dataset to it
from a numpy array.

::

>>> import h5py
>>> import numpy as np
>>> f = h5py.File("filename.h5","w")
>>> a = np.arange(10)
>>> fa = f.create_dataset("integer_array",(10,),dtype='i4')
>>> fa[:] = a
>>> f.close()


Dump HDF5 file
--------------

We can use two of the programs of the hdf5 library to have a look at the file
content:

+ The first program is **h5ls**, and is used to have a list of the file content.
+ The second program is **h5dump** and is used to dump in a human readable
  format the file content. Using the *-x* option, we can dump the file in XML
  format.

The library provides other programs, among which the **h5repack**, which can
copy an input HDF5 file to another HDF5 file with optimized or compressed
datasets.

Dump the HDF5 file
------------------

::

 $ h5ls filename.h5
 integer_array            Dataset {10}

 $ h5dump filename.h5 
 HDF5 "filename.h5" {
 GROUP "/" {
    DATASET "integer_array" {
       DATATYPE  H5T_STD_I32LE
       DATASPACE  SIMPLE { ( 10 ) / ( 10 ) }
       DATA {
       (0): 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
       }
    }
 }
 }

Read data from HDF5 file
------------------------

To read back data from the HDF5 file:

::

 >>> import h5py
 >>> f = h5py.File("filename.h5")
 >>> a = f["integer_array"][...]
 >>> type(a)
 <type 'numpy.ndarray>
 >>> a
 array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9], dtype=int32)
 >>> f.close()

HDF5 resizable dataset
----------------------

You can create resizable dataset (think time axis) when we create a new
dataset:

::

 >>> import h5py
 >>> f = h5py.File("resizable_array.h5","w")
 >>> dset_time = f.create_dataset('time', (1,), maxshape=(None,))
 >>> dset_time[0] = 1.0
 >>> dset_time.resize((2,))
 >>> dset_time[1] = 2.0
 >>> def add_a_time(dd,t):
 ...    last = dd.shape[0]
 ...    dd.resize((last+1,))
 ...    dd[last] = t
 ...
 >>> add_a_time(dset_time,3.0)
 >>> add_a_time(dset_time,4.0)
 >>> f.close()

HDF5 unlimited dimension result
-------------------------------

The file content with h5dump is:

::

 $ h5dump resizable_array.h5 
 HDF5 "resizable_array.h5" {
 GROUP "/" {
    DATASET "time" {
       DATATYPE  H5T_IEEE_F32LE
       DATASPACE  SIMPLE { ( 4 ) / ( H5S_UNLIMITED ) }
       DATA {
       (0): 1, 2, 3, 4
       }
    }
 }
 }

Note the **H5S_UNLIMITED** dimension of the array.

HDF5 filter pipeline
--------------------

HDF5 has the concept of a *filter pipeline*.

+ A pipeline is a series of operation that are performed on each data chunck
  when it is writtem on disk.
+ When the data are read from disk, the pipeline is reversed.

A commonly used filter is the **GZIP** one, which compress data before
writing on disk.
Other filters allow for checksumming, pre-shuffling of data to increase the
compression performances.

The API allows for a common filter interface and the load at runtime of
a user DSO to allow custom filtering. Think of a crypt filter.

Example of creating compressed dataset
--------------------------------------

::

 >>> import h5py
 >>> f = h5py.File("compressed.h5","w")
 >>> dset = f.create_dataset('var', (100,100,1),
 ...                         maxshape=(100,100,None,),
 ...                         chunks=(100,100,1),
 ...                         dtype='f4', compression='gzip')
 >>> dset[...] = 42.0
 >>> f.close()
 >>> f = h5py.File("uncompressed.h5","w")
 >>> dset = f.create_dataset('var', (100,100,1), chunks=(100,100,1),
 ...                         maxshape=(100,100,None,), dtype='f4')
 >>> dset[...] = 42.0
 >>> f.close()
 $ ls -sh *compressed.h5
 8K  compressed.h5
 44K uncompressed.h5

HDF5 Groups
-----------

Let us now have a look at groups, which permits us to have hierarchical
structures in files.

::

  >>> import h5py
  >>> import numpy as np
  >>> f = h5py.File("groups.h5","w")
  >>> basegroup = f.create_group("base_group")
  >>> basegroup
  <HDF5 group "/base_group" (0 members)>
  >>> nested1 = basegroup.create_group("nested1")
  >>> nested2 = basegroup.create_group("nested2")
  >>> basegroup
  <HDF5 group "/base_group" (2 members)>
  >>> dset = nested1.create_dataset("integer_array",(10,),dtype='i4')
  >>> dset[:] = np.arange(10)
  >>> dset = nested2.create_dataset("integer_array",(10,),dtype='i4')
  >>> dset[:] = np.arange(10)*2
  >>> f.close()

Output file listing
-------------------

::

  $ h5ls -lr groups.h5 
  /                        Group
  /base_group              Group
  /base_group/nested1      Group
  /base_group/nested1/integer_array Dataset {10}
  /base_group/nested2      Group
  /base_group/nested2/integer_array Dataset {10}

HDF5 Attributes
---------------

Last piece of the HDF5 data model are attributes. They can be accessed at
group or dataset level.

::

 >>> import h5py
 >>> f = h5py.File("attributes.h5","w")
 >>> basegroup = f.create_group("base_group")
 >>> basegroup.attrs['description'] = 'Here we put all stuff'
 >>> nested_two_lev = basegroup.create_group('lev1/lev2')
 >>> nested_one_lev = basegroup['lev1']
 >>> nested_one_lev.attrs['level_number'] = 1
 >>> nested_two_lev.attrs['level_number'] = 2
 >>> dset = nested_one_lev.create_dataset("integer_array",(10,),dtype='i4')
 >>> dset.attrs['units'] = 'm'
 >>> f.close()


HDF5 file with attributes
-------------------------
::

 $ h5ls -lrv attributes.h5
 /base_group              Group
     Attribute: description scalar
         Type:      variable-length null-terminated UTF-8 string
         Data:  "Here we put all stuff"
 /base_group/lev1         Group
     Attribute: level_number scalar
         Type:      native long
         Data:  1
 /base_group/lev1/integer_array Dataset {10/10}
     Attribute: units scalar
         Type:      variable-length null-terminated UTF-8 string
         Data:  "m"
 /base_group/lev1/lev2    Group
     Attribute: level_number scalar
         Type:      native long
         Data:  2

netCDF Solution
---------------

The netCDF library is a more array oriented storage format, which lacks some
of the features of the HDF5 library, but has a simple interface.
It is described as:

*... Software libraries and self-describing, machine-independent
data formats that support the creation, access, and sharing of
array-oriented scientific data ...*

It is available from Unidata, one of the University Corporation for
Atmospheric Research (UCAR)'s Community Programs (UCP).
The latest implementation of the netCDF file format can be found 
from Unidata_

   http://www.unidata.ucar.edu/software/netcdf

.. _Unidata: http://www.unidata.ucar.edu/software/netcdf

Who uses netCDF
---------------

+ Earth Science Modeling data
+ NOAA's Climate Analysis Branch (CAB)
+ EUMETSAT Satellite data distribution
+ NASA's Halogen Occultation Experiment
+ The Woods Hole Field Center of the USGS
+ The CSIRO Division of Atmospheric Research in Australia
+ General purpose finite element data model at SANDIA
+ Multi-body dynamics analysis systems
+ Analytical Data Interchange Protocols for chromatography and
  mass spectrometry
+ The Positron Imaging Laboratories and the Neuro-Imaging Laboratory
  of the Montreal Neurological Institute
+ molecular dynamics and the simulation of biomolecules for the
  AMBER project
+ Culham Centre for Fusion Energy

Disk Formats
------------

The file on disk can have different formats:

+ NetCDF 3 classic : binary file with maximum size of 2 GB
+ NetCDF 3 64 bit offset : binary file with max 4GB per variable
+ NetCDF 4 : HDF5 file format with netCDF API on top
+ NetCDF 4 Classic : HDF5 file format with some limitations

Data Model
----------

With respect to the HDF5 data model, the netCDF has similar basic objects

- *GROUP* : each file contains one or more groups. They have distinct
  namespaces, and can be considered equivalent to on disk directores.
  Any group can contains multiple GROUPs. If not defined a GROUP, all
  other objects are in the ROOT group.
- *DIMENSION* : an integer number, fixed or growing, defining a
  dimensionality
- *DATATYPE* : User defined or primitive data structure
- *VARIABLE* : The actual data which has any number of dimensions
  and any number of attrybutes and one *DATATYPE*
- *ATTRIBUTE* : any data type associated either to a *GROUP* or to
  a *VARIABLE*

We will use the simple netCDF4_ Python interface from now on to illustrate
the basic concepts in a netCDF file usage.

   https://unidata.github.io/netcdf4-python

.. _netCDF4: <https://unidata.github.io/netcdf4-python>

Writing a netCDF file
---------------------

Unidata supports netCDF APIs in C, C++, Fortran, and Java. Different projects
provide netCDF API for Python , perl , Ruby , R , Matlab , IDL.

::

 >>> from netCDF4 import Dataset
 >>> import numpy as np
 >>> f = Dataset('test.nc', 'w', format='NETCDF4')
 >>> f.description = 'bogus example script'
 >>> subgroup = f.createGroup('subgroup')
 >>> n1 = f.createDimension('n1', 3)
 >>> n2 = f.createDimension('n2', 3)
 >>> var = subgroup.createVariable('var','i4',(n1._name,n2._name))
 >>> var.comment = 'A comment'
 >>> a = np.ones((3,3))
 >>> var[...] = a
 >>> f.close()

Reading data from a netCDF file
-------------------------------

::

 >>> from netCDF4 import Dataset
 >>> f = Dataset('test.nc')
 >>> subgroup = f.groups['subgroup']
 >>> var = subgroup.variables['var']
 >>> var.comment
 'A comment'
 >>> a = var[...]
 >>> a
 array([[1, 1, 1],
        [1, 1, 1],
        [1, 1, 1]], dtype=int32)
 >>> f.close()

Dumping netCDF file
-------------------

We can use the **ncdump** program fron the netCDF library to transform the
binary file content into a human readable format, and conversely the **ncgen**
program to create a netCDF binary file from a textual representation of a file.

The netCDF library provides a third program **nccopy** which can be used to
create a new file from an existing one, changing the on-disk format and the
chunking strategy.

Dumping a file
--------------

::

 netcdf test {
 dimensions:
         n1 = 3 ;
         n2 = 3 ;
 // global attributes:
                 :description = "bogus example script" ;
 group: subgroup {
   variables:
         int var(n1, n2) ;
                 var:comment = "A comment" ;
   data:
    var =
  1, 1, 1,
  1, 1, 1,
  1, 1, 1 ;
  } // group subgroup
 }


Writing a file: Fortran interface (1) : Create
----------------------------------------------

::

 program example_netcdf
   use netcdf
   integer :: ncid , idims(3) , ivar , errval
   integer :: nx , ny
   integer :: istart(3) , icount(3)
   character(len=256) :: filename
   double precision , allocatable :: matrix(:,:)
   integer :: mode = NF90_NOCLOBBER
   [...]
   errval = nf90_create(filename, mode, ncid)
   if ( errval /= NF90_NOERR ) then
     print *, nf90_strerror(errval)
     stop
   end if

Writing a file: Fortran interface (2) : Define
----------------------------------------------

::

   [...]
   errval = nf90_put_att(ncid, nf90_global, 'title', 'The title')
   [...]
   errval = nf90_def_dim(ncid, 'X', nx, idims(1))
   [...]
   errval = nf90_def_dim(ncid, 'Y', ny, idims(2))
   [...]
   errval = nf90_def_dim(ncid, 'time', nf90_unlimited, idims(3))
   [...]
   errval = nf90_def_var(ncid, 'var', nf90_double, idims, ivar)
   [...]
   errval = nf90_put_att(ncid, ivar, 'units', 'dimensionless')
   [...]
   errval = nf90_enddef(ncid)
   [...]

Writing a file: Fortran interface (3) : Write
---------------------------------------------

::

   [...]
   do it = 1 , nt
     [...]
     istart(1) = it
     istart(2) = 1
     istart(3) = 1
     icount(1) = 1
     icount(2) = ny
     icount(3) = nx
     errval = nf90_put_var(ncid, ivar, matrix, istart, icount)
     [...]
   end do
   [...]
   errval = nf90_close(ncid)
   [...]

Afternoon Lab
-------------

We will install netCDF library with MPI capability on top of the HDF5.

We will examine how to use netcdf library to perform parallel IO.

Different strategies must be evaluated, and we will examine how chunking
affects performances.

