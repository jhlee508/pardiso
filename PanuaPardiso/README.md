# Panua-Pardiso

[Panua](https://panua.ch/) Pardiso Solver (https://panua.ch/pardiso/)


## Install & Build
- [LAPACK/BLAS](https://www.netlib.org/lapack/)
- [cpny](https://github.com/rogersce/cnpy) (only for using `.npz` or `.npy` Numpy files in C/C++)


## Setup
Linking the libraries to execute [Pardiso](https://panua.ch/pardiso/)

```Bash
g++ <source file> -o <executable> -L<path/to/pardiso/lib/dir> -lpardiso -L<path/to/LAPACK&BLAS/dir> -llapack -lblas -lgfortran -fopenmp -lpthread -lm
```

For using [Cnpy](https://github.com/rogersce/cnpy) with [Pardiso](https://panua.ch/pardiso/)

```Bash
g++ <source file> -o <executable> -L<path/to/cpny/lib/dir> -lcnpy -lz --std=c++11 -L<path/to/pardiso/dir> -lpardiso -llapack -lblas -lgfortran -fopenmp -lpthread -lm
```

## Usage
```Bash
source setup.sh
```
```Bash
sh compile.sh
./pardiso
```
