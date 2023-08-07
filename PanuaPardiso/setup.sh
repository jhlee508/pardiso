#! /bin/sh

# Library location and ETC
export LD_LIBRARY_PATH=/home/n4/jaehwan/pardiso/PanuaPardiso/lib_a4:$LD_LIBRARY_PATH # Pardiso
export LD_LIBRARY_PATH=/home/n4/jaehwan/pardiso/PanuaPardiso/cnpy/lib:$LD_LIBRARY_PATH # CNPY

# Panua Setting
export OMP_NUM_THREADS=16 # OpenMP Thread Numbers
export PARDISOLICMESSAGE=1 # Panua License Message Invalidate
export PARDISO_LIC_PATH=/home/n4/jaehwan/pardiso/PanuaPardiso/license # Panua Pardiso License