/* -------------------------------------------------------------------- */
/*      Example program to show the use of the "PARDISO" routine        */
/*      on for unsymmetric linear systems                               */
/* -------------------------------------------------------------------- */
/*      This program can be downloaded from the following site:         */
/*      http://www.pardiso-project.org                                  */
/*                                                                      */
/*  (C) Olaf Schenk, Institute of Computational Science                 */
/*      Universita della Svizzera italiana, Lugano, Switzerland.        */
/*      Email: olaf.schenk@usi.ch                                       */
/* -------------------------------------------------------------------- */

// C++ compatible

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

// CNpy: Numpy in C
#include "cnpy/cnpy.h"

using namespace std;

/* PARDISO prototype. */
extern "C" void pardisoinit (void   *, int    *,   int *, int *, double *, int *);
extern "C" void pardiso     (void   *, int    *,   int *, int *,    int *, int *, 
                  double *, int    *,    int *, int *,   int *, int *,
                     int *, double *, double *, int *, double *);
extern "C" void pardiso_chkmatrix  (int *, int *, double *, int *, int *, int *);
extern "C" void pardiso_chkvec     (int *, int *, double *, int *);
extern "C" void pardiso_printstats (int *, int *, double *, int *, int *, int *, double *, int *);


int main( void ) 
{
    /* Samsung Display Sample Matrix */
    cnpy::NpyArray data = cnpy::npy_load("../sample_npz/matrix_2037609/data.npy"); // A data
    cnpy::NpyArray col_idx = cnpy::npy_load("../sample_npz/matrix_2037609/col_idx.npy"); // A col_idx
    cnpy::NpyArray row_ptr = cnpy::npy_load("../sample_npz/matrix_2037609/row_ptr.npy"); // A row_ptr
    cnpy::NpyArray b_array = cnpy::npy_load("../sample_npz/matrix_2037609/b.npy"); // b array

    printf("A data Size: %ld\n", data.shape[0]);
    printf("A col_idx Size: %ld\n", col_idx.shape[0]);
    printf("A row_ptr Size: %ld\n", row_ptr.shape[0]);
    printf("b array Size: %ld\n", b_array.shape[0]);
    
    double*  a = data.data<double>();
    int*     ja = col_idx.data<int>();
    int*     ia = row_ptr.data<int>();
    double*  b = b_array.data<double>();

    int      n = (int)b_array.shape[0];

    printf("Size of b array (n): %d\n", n);

    /* Number of Non-zeros and Matrix type */
    int      nnz = ia[n];
    int      mtype = 11;        /* Real unsymmetric matrix */

    printf("Number of Non-zeros: %d\n", nnz);

    /* RHS and solution vector 'x' */
    double* x = new double[n];  //  double   x[n];
    int      nrhs = 1;          /* Number of right hand sides. */

    /* Internal solver memory pointer pt,                  */
    /* 32-bit: int pt[64]; 64-bit: long int pt[64]         */
    /* or void *pt[64] should be OK on both architectures  */ 
    void    *pt[64];

    /* Pardiso control parameters. */
    int      iparm[64];
    double   dparm[64];
    int      solver;
    int      maxfct, mnum, phase, error, msglvl;

    /* Number of processors. */
    int      num_procs;

    /* Auxiliary variables. */
    char    *var;
    int      i;

    double   ddum;              /* Double dummy */
    int      idum;              /* Integer dummy. */

/* -------------------------------------------------------------------- */
/* ..  Setup Pardiso control parameters and initialize the solvers      */
/*     internal adress pointers. This is only necessary for the FIRST   */
/*     call of the PARDISO solver.                                      */
/* ---------------------------------------------------------------------*/
      
    error = 0;
    solver = 0; /* use sparse direct solver */
    pardisoinit (pt,  &mtype, &solver, iparm, dparm, &error);

    if (error != 0)
    {
        if (error == -10 )
           printf("No license file found \n");
        if (error == -11 )
           printf("License is expired \n");
        if (error == -12 )
           printf("Wrong username or hostname \n");
         return 1;
    }
    else
        printf("[PARDISO]: License check was successful ... \n");
 

    /* Numbers of processors, value of OMP_NUM_THREADS */
    var = getenv("OMP_NUM_THREADS");
    if(var != NULL)
        sscanf( var, "%d", &num_procs );
    else {
        printf("Set environment OMP_NUM_THREADS to 1");
        exit(1);
    }
    iparm[2]  = num_procs;
   
    
    maxfct = 1;         /* Maximum number of numerical factorizations.  */
    mnum   = 1;         /* Which factorization to use. */
    
    msglvl = 1;         /* Print statistical information  */
    error  = 0;         /* Initialize error flag */


/* -------------------------------------------------------------------- */    
/* ..  Convert matrix from 0-based C-notation to Fortran 1-based        */
/*     notation.                                                        */
/* -------------------------------------------------------------------- */ 
    for (i = 0; i < n+1; i++) {
        ia[i] += 1;
    }
    for (i = 0; i < nnz; i++) {
        ja[i] += 1;
    }

    // /* Set right hand side to one. */
    // for (i = 0; i < n; i++) {
    //     b[i] = i;
    // }

/* -------------------------------------------------------------------- */
/*  .. pardiso_chk_matrix(...)                                          */
/*     Checks the consistency of the given matrix.                      */
/*     Use this functionality only for debugging purposes               */
/* -------------------------------------------------------------------- */
    
    pardiso_chkmatrix  (&mtype, &n, a, ia, ja, &error);
    if (error != 0) {
        printf("\nERROR in consistency of matrix: %d", error);
        exit(1);
    }

/* -------------------------------------------------------------------- */
/* ..  pardiso_chkvec(...)                                              */
/*     Checks the given vectors for infinite and NaN values             */
/*     Input parameters (see PARDISO user manual for a description):    */
/*     Use this functionality only for debugging purposes               */
/* -------------------------------------------------------------------- */

    pardiso_chkvec (&n, &nrhs, b, &error);
    if (error != 0) {
        printf("\nERROR  in right hand side: %d", error);
        exit(1);
    }

/* -------------------------------------------------------------------- */
/* .. pardiso_printstats(...)                                           */
/*    prints information on the matrix to STDOUT.                       */
/*    Use this functionality only for debugging purposes                */
/* -------------------------------------------------------------------- */

    pardiso_printstats (&mtype, &n, a, ia, ja, &nrhs, b, &error);
    if (error != 0) {
        printf("\nERROR right hand side: %d", error);
        exit(1);
    }
 
/* -------------------------------------------------------------------- */    
/* ..  Reordering and Symbolic Factorization.  This step also allocates */
/*     all memory that is necessary for the factorization.              */
/* -------------------------------------------------------------------- */ 
    // phase = 11; 

    // pardiso (pt, &maxfct, &mnum, &mtype, &phase,
    //          &n, a, ia, ja, &idum, &nrhs,
    //          iparm, &msglvl, &ddum, &ddum, &error,  dparm);
    
    // if (error != 0) {
    //     printf("\nERROR during symbolic factorization: %d", error);
    //     exit(1);
    // }
    // printf("\nReordering completed ... ");
    // printf("\nNumber of nonzeros in factors  = %d", iparm[17]);
    // printf("\nNumber of factorization MFLOPS = %d", iparm[18]);
   
/* -------------------------------------------------------------------- */    
/* ..  Numerical factorization.                                         */
/* -------------------------------------------------------------------- */    
    // phase = 22;

    // pardiso (pt, &maxfct, &mnum, &mtype, &phase,
    //          &n, a, ia, ja, &idum, &nrhs,
    //          iparm, &msglvl, &ddum, &ddum, &error, dparm);
   
    // if (error != 0) {
    //     printf("\nERROR during numerical factorization: %d", error);
    //     exit(2);
    // }
    // printf("\nFactorization completed ...\n ");

/* -------------------------------------------------------------------- */    
/* ..  Back substitution and iterative refinement.                      */
/* -------------------------------------------------------------------- */    
    // phase = 33;

    // iparm[7] = 1;       /* Max numbers of iterative refinement steps. */

   
    // pardiso (pt, &maxfct, &mnum, &mtype, &phase,
    //          &n, a, ia, ja, &idum, &nrhs,
    //          iparm, &msglvl, b, x, &error,  dparm);
   
    // if (error != 0) {
    //     printf("\nERROR during solution: %d", error);
    //     exit(3);
    // }

    // printf("\nSolve completed ... ");
    // printf("\nThe solution of the system is: ");
    // for (i = 0; i < 20; i++) {
    //     printf("\n x [%d] = % f", i, x[i] );
    // }
    // printf ("\n");

/* -------------------------------------------------------------------- */    
/* ..  Total Solve Phase                                                      */
/* -------------------------------------------------------------------- */    
    phase = 13;

    iparm[7] = 1;       /* Max numbers of iterative refinement steps. */

   
    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
             &n, a, ia, ja, &idum, &nrhs,
             iparm, &msglvl, b, x, &error,  dparm);
   
    if (error != 0) {
        printf("\nERROR during solution: %d", error);
        exit(3);
    }

    printf("\nSolve completed ... ");
    printf("\nThe solution of the system is: ");
    for (i = 0; i < 20; i++) {
        printf("\n x[%d] = % f", i, x[i]);
    }
    printf ("\n");

    printf("Saving solution in npy file\n");
    cnpy::npy_save("../sample_npz/matrix_2037609/panua_sol.npy", &x[0], {2037609}, "w"); // x array length(n)

/* -------------------------------------------------------------------- */    
/* ..  Convert matrix back to 0-based C-notation.                       */
/* -------------------------------------------------------------------- */ 
    for (i = 0; i < n+1; i++) {
        ia[i] -= 1;
    }
    for (i = 0; i < nnz; i++) {
        ja[i] -= 1;
    }

/* -------------------------------------------------------------------- */    
/* ..  Termination and release of memory.                               */
/* -------------------------------------------------------------------- */ 
    phase = -1;                 /* Release internal memory. */

    pardiso (pt, &maxfct, &mnum, &mtype, &phase,
             &n, &ddum, ia, ja, &idum, &nrhs,
             iparm, &msglvl, &ddum, &ddum, &error,  dparm);

    return 0;
} 
