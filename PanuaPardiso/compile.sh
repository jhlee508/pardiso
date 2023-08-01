# Pardiso Compile with LAPACK and BLAS
#g++ pardiso_unsym.cpp -o pardiso_unsym -Llib -lpardiso -llapack -lblas -lgfortran -fopenmp -lpthread -lm

# Pardiso Compile with LAPACK and BLAS + CNpy (Numpy in C++)
g++ pardiso_samsung.cpp -o pardiso -Lcnpy/lib -lcnpy -lz --std=c++11 -Llib -lpardiso -llapack -lblas -lgfortran -fopenmp -lpthread -lm

# Library location and ETC
export LD_LIBRARY_PATH=/home/n4/jaehwan/pardiso/panua-pardiso/lib:$LD_LIBRARY_PATH
export OMP_NUM_THREADS=32
export PARDISOLICMESSAGE=1