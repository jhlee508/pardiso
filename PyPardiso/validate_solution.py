import numpy as np
import scipy

print("Loading Panua Solution x ...") 
panua_sol = np.load('../sample_npz/matrix_2037609/panua_sol.npy')

print("Loading Intel MKL Solution x ...\n") 
mkl_sol = np.load('../sample_npz/matrix_2037609/mkl_sol.npy')

print("Panua vs MKL")
for i in range(10):
    print("x[{}] : ".format(i), end='')
    print(panua_sol[i], "vs", mkl_sol[i])

abs_err = max([abs(i - j) for i, j in zip(panua_sol, mkl_sol)])
rel_err = max([abs(i - j) / i for i, j in zip(panua_sol, mkl_sol) if i != 0])
print("\nAbsolute Error:", abs_err)
print("Relative Error:", rel_err)

if np.testing.assert_array_almost_equal(panua_sol, mkl_sol, decimal=6): # abs(desired-actual) < 1.5 * 10**(-decimal)
    pass
else:    
    print("\nValidation Successed!")