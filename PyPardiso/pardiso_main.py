# coding: utf-8
import pypardiso

import numpy as np
import time

import scipy.sparse as sp
import scipy.io as sio
import scipy

import mkl
mkl.set_dynamic(0)
mkl.set_num_threads(32)


mat = sio.loadmat("../sample_matrices/solvertest1.mat") # load sample matrix
A, b = mat['J'], mat['R'].flatten()
n = A.get_shape()[0]

ps = pypardiso.PyPardisoSolver()
ps.set_statistical_info_on()

# Iparm Setup
ps.set_iparm(0 + 1, 0) # Use default values (iparm[0]: default = 0)

# input iparm
ps.set_iparm(1 + 1, 2) # Fill-in reducing ordering (default = 2)

ps.set_iparm(7 + 1, 5) # Iterative Refinement Step (iparm[7]: default = 0)
ps.set_iparm(9 + 1, 13) # Pivoting Perturbation(iparm[9]: default = 13)

ps.set_iparm(10 + 1, 1) # Scaling vectors (default = 1 for nonsymmetric)
ps.set_iparm(12 + 1, 1) # Improved accuracy using (non-) symmetric weighted matching (default = 1 for nonsymmetric)
ps.set_iparm(27 + 1, 0) # Single(1) or Double(0) Precision
ps.set_iparm(26 + 1, 0) # Matrix Checker (default = 0)
ps.set_iparm(59 + 1, 0) # IC(0) or OOC(1) mode (iparm[60]: default = 0)

#output iparm
ps.set_iparm(17 + 1, -1) # Report the number of non-zero elements in the factors
ps.set_iparm(18 + 1, -1) # Report number of floating point operations

start_time = time.time()
x = pypardiso.spsolve(A, b, solver=ps) # phase(12): factorize + phase(33): solve 
print("--- Elapsed Time: %.4f seconds ---" % (time.time() - start_time))
# print("Number of non-zeros:", ps.get_iparm(18))
# print("GFlops:", ps.get_iparm(19) / 1000)

print("The solution of the system is: ")
print(x)
print("Saving PyPardiso MKL Solution ...")
np.save('../sample_npz/solvertest1/solvertest1_mkl_sol', x)


if np.testing.assert_array_almost_equal(A*x, b, decimal=6): # abs(desired-actual) < 1.5 * 10**(-decimal)
    print("\nValidation2 Successed!")

print("\nValidation3")
sol = mat['sol'].flatten()
sol_new = x
abs_err = max([abs(i - j) for i, j in zip(sol, sol_new)])
rel_err = max([abs(i - j) / i for i, j in zip(sol, sol_new) if i != 0])
print("Absolute Error:", abs_err)
print("Relative Error:", rel_err)
