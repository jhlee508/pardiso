#include <iostream>
#include "StrumpackSparseSolverMPIDist.hpp"
#include "StrumpackSparseSolver.hpp"
#include "sparse/CSRMatrix.hpp"
#include "sparse/CSRMatrixMPI.hpp"

using namespace strumpack;

typedef double scalar;
typedef int integer;

int main(int argc, char* argv[ ]) {
    int thread_level, rank, P;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &thread_level);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &P);
    {   
        // construct an NxN CSR matrix with nnz nonzeros
        int N = 5, nnz = 5;      

        // define a block-row distributed CSR matrix
        int* dist = new int[P+1];

        // set dist such that processor p owns rows [dist[p], dist[p+1]) of the sparse matrix
        for (int p = 0; p < P; p++) dist[p] = 2 * p;
        dist[P] = N;

        // local_n is the number of rows of the input matrix assigned to me
        // set the sparse matrix row pointers in row_ptr
        int local_n   = dist[rank+1] - dist[rank];
        int* row_ptr  = new int[local_n + 1];
        row_ptr[0] = 0;
        row_ptr[1] = 1;
        row_ptr[2] = 2;
        row_ptr[3] = 3;
        row_ptr[4] = 4;
        row_ptr[5] = 5;

        // set the sparse matrix column indices in col_ind
        int local_nnz = row_ptr[local_n + 1] - row_ptr[0];
        int* col_ind  = new int[local_nnz];
        col_ind[0] = 0;
        col_ind[1] = 1;
        col_ind[2] = 2;
        col_ind[3] = 3;
        col_ind[4] = 4;

        double* val   = new double[local_nnz];
        // set the matrix nonzero value in val
        val[0] = 1.;
        val[1] = 2.;
        val[2] = 3.;
        val[3] = 4.;
        val[4] = 5.;

        // local part of solution
        double* x = new double[local_n];            
        // local part of rhs 
        double* b = new double[local_n];             
        // set the rhs
        for (int i = 0; i < local_n; i++) b[i] = 1.;     

        // create solver object
        StrumpackSparseSolverMPIDist<scalar, integer> sp(MPI_COMM_WORLD);
        // set options
        /* PARMETIS reordering */
        sp.options().set_reordering_method(ReorderingStrategy::PARMETIS);
        /* parse command line options */
        sp.options().set_from_command_line(argc, argv);

        // set the matrix (copy)
        sp.set_distributed_csr_matrix(local_n, row_ptr, col_ind, val, dist);

        // solve Ax=b
        sp.solve(b, x);

        // print output
        if (!rank) std::cout << ">>> x = [" << std::flush;
        for (int p = 0; p < rank; p++) MPI_Barrier(MPI_COMM_WORLD);
        for (int i = 0; i < N; i++) std::cout << x[i] << " " << std::flush;
        for (int p = rank; p < P + 1; p++) MPI_Barrier(MPI_COMM_WORLD);
        if (!rank) std::cout << "];" << std::endl;

        // check residual/error, cleanup
        delete[] dist;
        delete[] row_ptr;
        delete[] col_ind;
        delete[] val;
        delete[] x;
        delete[] b;
    }
    scalapack::Cblacs_exit(1);
    MPI_Finalize();
    return 0;
}