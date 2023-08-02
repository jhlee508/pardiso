#! /bin/sh

# Library location and ETC
export LD_LIBRARY_PATH=/home/n4/jaehwan/pardiso/PanuaPardiso/lib:$LD_LIBRARY_PATH # Pardiso
export LD_LIBRARY_PATH=/home/n4/jaehwan/pardiso/PanuaPardiso/cnpy/lib:$LD_LIBRARY_PATH # CNPY
export OMP_NUM_THREADS=32
export PARDISOLICMESSAGE=1