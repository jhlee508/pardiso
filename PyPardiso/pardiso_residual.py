import scipy
import pypardiso
import numpy as np
import time

import mkl
mkl.set_dynamic(0)
mkl.set_num_threads(16)

mat = scipy.io.loadmat('../sample_matrices/solvertest1.mat')
A, b = mat['J'], mat['R'].flatten()
n = A.get_shape()[0]
x = pypardiso.spsolve(A, b)

if A.dtype == np.float64: eps = 5e-16
else: eps = 5e-7

Axmb_norm_I = np.linalg.norm(A.dot(x) - b, ord=np.inf)
Anorm1 = scipy.sparse.linalg.norm(A, ord=1)
AnormI = scipy.sparse.linalg.norm(A, ord=np.inf)
xnorm1 = np.linalg.norm(x, ord=1)
xnormI = np.linalg.norm(x, ord=np.inf)
bnormI = np.linalg.norm(b, ord=np.inf)

resid = Axmb_norm_I / ((AnormI * xnormI + bnormI) * n * eps)

print("||Ax-b||_oo/(eps*(||A||_oo*||x||_oo+||b||_oo)*N) = ", f"{resid:.3e}")
print("||Ax-b||_oo  . . . . . . . . . . . . . . . . . . = ", f"{Axmb_norm_I:.3e}")
print("||A||_oo . . . . . . . . . . . . . . . . . . . . = ", f"{AnormI:.3e}")
print("||A||_1  . . . . . . . . . . . . . . . . . . . . = ", f"{Anorm1:.3e}")
print("||x||_oo . . . . . . . . . . . . . . . . . . . . = ", f"{xnormI:.3e}")
print("||x||_1  . . . . . . . . . . . . . . . . . . . . = ", f"{xnorm1:.3e}")
print("||b||_oo . . . . . . . . . . . . . . . . . . . . = ", f"{bnormI:.3e}")
print()

print("Max value of x: ", max(x))
print("Min value of x: ", min(x))
print()

Ax = A * x

max_rel_err = 0.0
max_abs_err = 0.0
max_rel_err_index = 0
max_abs_err_index = 0
for i in range(len(Ax)):
  if max_abs_err < (abs(Ax[i] - b[i])):
    max_abs_err = (abs(Ax[i] - b[i]))
    max_abs_err_index = i
  if b[i] != 0.0:
    if max_rel_err < (abs(Ax[i] - b[i]) / b[i]):
      max_rel_err = (abs(Ax[i] - b[i]) / b[i])
      max_rel_err_index = i

print("Max Abs. Error:", max_abs_err)
print("Result Value:", Ax[max_abs_err_index])
print("Actual Value:", b[max_abs_err_index])
print()
print("Max Rel. Error:", max_rel_err)
print("Result Value:", Ax[max_rel_err_index])
print("Actual Value:", b[max_rel_err_index])
print()

if np.testing.assert_array_almost_equal(A*x, b, decimal=6): # abs(desired-actual) < 1.5 * 10**(-decimal)
  pass
else:    
  print("\nValidation Successed!")