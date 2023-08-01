import numpy as np
import scipy

print("Loading Panua Solution x ...") 
panua_sol = np.load('../sample_npz/solvertest1/solvertest1_panua_sol.npy')

print("Loading Intel MKL Solution x ...") 
mkl_sol = np.load('../sample_npz/solvertest1/solvertest1_mkl_sol.npy')

abs_err = max([abs(i - j) for i, j in zip(panua_sol, mkl_sol)])
rel_err = max([abs(i - j) / i for i, j in zip(panua_sol, mkl_sol) if i != 0])
print("Absolute Error:", abs_err)
print("Relative Error:", rel_err)

if np.testing.assert_array_almost_equal(panua_sol, mkl_sol, decimal=6): # abs(desired-actual) < 1.5 * 10**(-decimal)
    print("\nValidation Successed!")