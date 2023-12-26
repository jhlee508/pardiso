#include <iostream>
#include "StrumpackSparseSolver.hpp"
#include "sparse/CSRMatrix.hpp"
#include "sparse/CSRMatrixMPI.hpp"

using namespace strumpack;

typedef double scalar;
typedef int integer;


int main(int argc, char* argv[]) {
    // construct an NxN CSR matrix with nnz nonzeros
    int N = 5, nnz = 5;                        
    // N+1 integers
    int* row_ptr = new int[N+1];
    // nnz integers               
    int* col_ind = new int[nnz];
    // nnz doubles
    double* val = new double[nnz];
    // will hold the solution vector with 0
    double* x = new double[N]();        
    // set a right-hand side b
    double* b = new double[N]; 

    // fill in the matrix A as 
    // A = [1 0 0 0 0
    //      0 2 0 0 0
    //      0 0 3 0 0
    //      0 0 0 4 0
    //      0 0 0 0 5];
    val[0] = 1.;  col_ind[0] = 0;  row_ptr[0] = 0;
    val[1] = 2.;  col_ind[1] = 1;  row_ptr[1] = 1;
    val[2] = 3.;  col_ind[2] = 2;  row_ptr[2] = 2;
    val[3] = 4.;  col_ind[3] = 3;  row_ptr[3] = 3;
    val[4] = 5.;  col_ind[4] = 4;  row_ptr[4] = 4;
    row_ptr[5] = 5;
    
    // fill in the right-hand side b as
    // b = [1 1 1 1 1];   
    for (int i = 0; i < N; i++) b[i] = 1.;      

    // create solver object
    StrumpackSparseSolver<scalar, integer> sp;
    // set options
    sp.options().set_rel_tol(1e-10);                 
    sp.options().set_gmres_restart(10); 
    /*  enable HSS compression, see HSS Preconditioning */
    sp.options().set_compression(CompressionType::HSS);
    /* parse command line options */
    sp.options().set_from_command_line(argc, argv);
    
    // set the matrix (copy)
    sp.set_csr_matrix(N, row_ptr, col_ind, val);
    // reorder matrix
    sp.reorder();
    // numerical factorization
    sp.factor();
    // solve Ax=b
    sp.solve(b, x);

    // print output 
    // x = [1 0.5 0.333333 0.25 0.2 ];
    std::cout << ">>> x = [" << std::flush;
    for (int i = 0; i < N; i++) std::cout << x[i] << " " << std::flush;
    std::cout << "];" << std::endl;
    
    // check residual/error or cleanup
    delete[] row_ptr;
    delete[] col_ind;
    delete[] val;
    delete[] x;
    delete[] b;

    return 0;
}