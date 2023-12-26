# STRUMPACK (STRUctured Matrix PACKage)
This describes how to setup for [**STRUMPACK (STRUctured Matrix PACKage)**](https://github.com/pghysels/STRUMPACK).

## TABLE OF CONTENTS
- [DEPENDENCIES](#dependencies)
	- [MPI & OpenMP](#mpi--openmp)
	- [LAPACK/BLAS](#lapackblas)
	- [METIS](#metis)
	- [ParMETIS](#parmetis-optional)
	- [ScaLAPACK](#scalapack)
	- [CUDA](#cuda-cublas-and-cusolver-optional)
	- [ZFP](#zfp)
	- [ButterflyPACK](#butterflypack)
	- [SLATE](#slate)
- [SETUP](#setup)
- [EXAMPES](#examples)
- [USAGE](#usage)


## DEPENDENCIES
Strumpack needs the following dependencies including **MPI** and **OpenMP**.
- **LAPACK/BLAS**
- **METIS** (>= 5.1.0) 
- **ParMETIS** (optional, only used when compiling with MPI)
- **ScaLAPACK** (included in **Intel oneMKL**)
- **CUDA**, **cuBLAS** and **cuSOLVER** (optional, to accelerate sparse direct solver)
- **ZFP** (>= 0.5.5)
- **ButterflyPACK** (>= 1.2.0)


### MPI & OpenMP

To disable **MPI** or **OpenMP**, add options as below.
```bash
cmake .. \
	-DSTRUMPACK_USE_MPI=OFF \
	-DSTRUMPACK_USE_OPENMP=OFF
```


### LAPACK/BLAS
If **BLAS** or **LAPACK** is not found automatically, you can specify the libraries as below.

```bash
cmake .. \
	-DTPL_BLAS_LIBRARIES=path/to/blas/lib \
	-DTPL_LAPACK_LIBRARIES=/path/to/lapack/lib
```


### METIS

To install **METIS** (>= 5.1.0), follow as below.

```bash
wget http://glaros.dtc.umn.edu/gkhome/fetch/sw/metis/metis-5.1.0.tar.gz
tar -xvzf metis-5.1.0.tar.gz
cd metis-5.1.0

make config cc=gcc prefix=`pwd`/install
make install
```
```bash
export METIS_DIR=`pwd`/install
```


### ParMETIS (optional)

To install **ParMETIS** (optionally used when compiling with MPI), follow as below.
```bash
wget http://glaros.dtc.umn.edu/gkhome/fetch/sw/parmetis/parmetis-4.0.3.tar.gz
tar -xvzf parmetis-4.0.3.tar.gz
cd parmetis-4.0.3

make config cc=mpicc prefix=`pwd`/install
make install
```
```bash
export ParMETIS_DIR=`pwd`/install
```

### CUDA, cuBLAS and cuSOLVER (optional)

To use **CUDA**, you can enable options as below.

```bash
cmake .. \
	-DSTRUMPACK_USE_CUDA=ON \
	-DCMAKE_CUDA_ARCHITECTURES="70" # for V100
```


### ScaLAPACK

You can install **ScaLAPACK** individually as below.

```bash
git clone https://github.com/Reference-ScaLAPACK/scalapack.git

cd scalapack
mkdir build install && cd build

cmake .. \
	-DCMAKE_INSTALL_PREFIX=../install \
	-DBUILD_SHARED_LIBS=ON

cmake --build . -j --target install
```

Specify path by setting environment variable as below.
```bash
export SCALAPACK_DIR=path/to/scalapack/install
```


### ZFP

Install **ZFP** as below.

```bash
git clone https://github.com/LLNL/zfp.git
cd zfp

mkdir build
cd build

cmake ..
cmake --build . --config Release

ctest # just for testing
```

Add **ZFP_DIR** PATH and enable when building.
```bash
export ZFP_DIR=/path/to/zfp/build
```

### ButterflyPACK

Install **ButterflyPACK** as below.

```bash
# Clone ButterflyPACK version 2.4.0
git clone -b v2.4.0 https://github.com/liuyangzhuan/ButterflyPACK.git
cd ButterflyPACK

mkdir build && cd build

cmake .. \
	-DTPL_BLAS_LIBRARIES=<Lib of the BLAS installation> \
	-DTPL_LAPACK_LIBRARIES=<Lib of the LAPACK installation> \
	-DTPL_SCALAPACK_LIBRARIES=<Lib of the SCALAPACK installation> \
	-DBUILD_SHARED_LIBS=ON \
	-DCMAKE_Fortran_COMPILER=mpif90 \
	-DCMAKE_CXX_COMPILER=mpicxx \
	-DCMAKE_C_COMPILER=mpicc \
	-DCMAKE_INSTALL_PREFIX=../install \
	-DCMAKE_BUILD_TYPE=Release

make install
```

### SLATE

Download [Release tar file](https://github.com/icl-utk-edu/slate/releases) and install **SLATE** as below.

```bash
tar -xvzf slate-2023.11.05.tar.gz

cd slate-2023.11.05
mkdir build && cd build

# For oneMKL or SYCL compiler support
source $HOME/intel/oneapi/setvars.sh
source $HOME/intel/oneapi/compiler/2024.0/env/vars.sh

export CXX=g++      # or your preferred C++ compiler (icpx -fsycl)
export FC=gfortran  # or your preferred Fortran compiler (ifort)
export CXXFLAGS=-DSLATE_HAVE_MT_BCAST

# If '-lscalapack' is not found
export LIBRARY_PATH=/path/to/scalapack/lib:$LIBRARY_PATH

cmake .. \
	-DCMAKE_INSTALL_PREFIX=../install \
	-Dblas=openblas \
	-Dgpu_backend=cuda \
	-DCMAKE_CUDA_ARCHITECTURES="70"

make && make install
```

Support for SLATE in STRUMPACK can be enabled as below
```bash
export blaspp_DIR=/path/to/blaspp/install/dir/
export lapackpp_DIR=/path/to/lapackpp/install/dir/
export slate_DIR=/path/to/slate/install/dir/

# To enable SLATE in STRUMPACK
cmake .. DTPL_ENABLE_SLATE=ON
```

## SETUP

To install **STRUMPACK** (ver. 7.1.0), follow as below.

```bash
tar -xvzf STRUMPACK-7.1.0.tar.gz

cd STRUMPACK-7.1.0
mkdir build && cd build

export METIS_DIR=`pwd`/../../metis-5.1.0/install
export ParMETIS_DIR=`pwd`/../../parmetis-4.0.3/install
export SCALAPACK_DIR=`pwd`/../../scalapack/install
export SCALAPACK_LIB=`pwd`/../../scalapack/install/lib/libscalapack.so
export ZFP_DIR=`pwd`/../../zfp/build
export ButterflyPACK_DIR=`pwd`/../../ButterflyPACK/install
export blaspp_DIR=`pwd`/../../slate-2023.11.05/install
export lapackpp_DIR=`pwd`/../../slate-2023.11.05/install
export slate_DIR=`pwd`/../../slate-2023.11.05/install

cmake .. \
	-DCMAKE_BUILD_TYPE=Release \
	-DCMAKE_INSTALL_PREFIX=../install \
	-DSTRUMPACK_USE_CUDA=ON \
	-DCMAKE_CUDA_ARCHITECTURES="70" \
	-DTPL_SCALAPACK_LIBRARIES="${SCALAPACK_LIB}"

### Other CMake options
# (To disable MPI or OpenMP)
# -DSTRUMPACK_USE_MPI=OFF 
# -DSTRUMPACK_USE_OPENMP=OFF

# (For LAPACK/BLAS)
# -DTPL_BLAS_LIBRARIES=path/to/blas/lib -DTPL_LAPACK_LIBRARIES=/path/to/lapack/lib

# (For CUDA, cuBLAS and cuSOLVER) 
# -DSTRUMPACK_USE_CUDA=ON -DCMAKE_CUDA_ARCHITECTURES="70"

make -j4 && make install

make examples -j4
make tests -j4
make test # can fail on login nodes
```

## EXAMPLES
To run examples follow as below.

```bash
cd examples/sparse

# Load libraries (e.g., mkl_gf_lp64, lapackpp)
source $HOME/intel/oneapi/setvars.sh 
export LD_LIBRARY_PATH=`pwd`/../../../../slate-2023.11.05/install/lib:$LD_LIBRARY_PATH

# run examples
OMP_NUM_THREADS=32 ./dexample 1000
```

## USAGE

To use STRUMPACK in your custom code, use a CMake build system as below.

```bash
# CMakeLists.txt
cmake_minimum_required(VERSION 3.13)
project(mytest LANGUAGES CXX)
 
find_package(STRUMPACK REQUIRED)
 
add_executable(main main.cpp)
target_link_libraries(main PRIVATE STRUMPACK::strumpack)
```

Build your custom code as below. FYR, **ScaLAPACK** libraries should locate in `$HOME/.local/scalapack` location.

```bash
mkdir mytest && cd mytest

# Define your CMakeLists.txt
vim CMakeLists.txt

mkdir build && cd build

export STRUMPACK_DIR=/path/to/strumpack/install

cmake ..
make
```

Run your code as below.

```bash
export OMP_NUM_THREADS=32

# Example code of StrumpackSparseSolver
./sp

# Example code of StrumpackSparseSolverMPIDist
mpirun -n 1 ./sp_mpi
```