#!/bin/bash

NPROC=8

for (( xsize = 1000; xsize < 10000; xsize = xsize + 1000 ))
do
  for (( ysize = 1000; ysize < 10000; ysize = ysize + 1000 ))
  do
    cat dimparam.in.template | \
      sed -e "s/gnx.*$/gnx = $xsize,/" \
          -e "s/gny.*$/gny = $ysize,/" > dimparam.in
    echo $xsize $ysize $(mpirun -np $NPROC ./allwrite dimparam.in | \
      grep Mean | cut -d " " -f 10)
  done
done
