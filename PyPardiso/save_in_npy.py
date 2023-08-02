import numpy as np
import scipy


mat = scipy.io.loadmat("../sample_matrices2/matrix_2037609.mat")
A, b = mat['J'], mat['R'].flatten()


## Saving
# A
A = A.tocsr()
print(type(A), A)
print("Saving Sparse Matrix A ...")
scipy.sparse.save_npz('../sample_npz/matrix_2037609/A.npz', A)
print("Saving Successed!\n")

# A.data (val)
print(type(A.data), len(A.data), A.data)
print("Saving Array A.data ...")
np.save('../sample_npz/matrix_2037609/data', A.data)
print("Saving Successed!\n")

# A.indices (col_idx)
print(type(A.indices), len(A.indices), A.indices)
print("Saving Array A.indices ...")
np.save('../sample_npz/matrix_2037609/col_idx', A.indices)
print("Saving Successed!\n")

# A.indptr (row_ptr)
print(type(A.indptr), len(A.indptr), A.indptr)
print("Saving Array A.indptr ...")
np.save('../sample_npz/matrix_2037609/row_ptr', A.indptr)
print("Saving Successed!\n")

# b
print(type(b), len(b), b)
print("Saving Array b...")
np.save('../sample_npz/matrix_2037609/b', b)
print("Saving Successed!\n")


## Loading
# A
print("Loading Sparse Matrix A ...") 
A_new = scipy.sparse.load_npz('../sample_npz/matrix_2037609/A.npz')
print("Shape:", A_new.shape[0])
print("NNZ:", A_new.getnnz())
print(type(A_new), A_new)
print("Loading Successed!\n")

# A.data (val)
print("Loading Array A.data ...")
data_new = np.load('../sample_npz/matrix_2037609/data.npy')
print(type(data_new), len(data_new), data_new)
print("Loading Successed!\n")

# A.indices (col_idx)
print("Loading Array A.indices ...")
col_idx_new = np.load('../sample_npz/matrix_2037609/col_idx.npy')
print(type(col_idx_new), len(col_idx_new), col_idx_new)
print("Loading Successed!\n")

# A.indptr (row_ptr)
print("Loading Array A.indptr ...")
row_ptr_new = np.load('../sample_npz/matrix_2037609/row_ptr.npy')
print(type(row_ptr_new), len(row_ptr_new), row_ptr_new)
print("Loading Successed!\n")

# b
print("Loading Array b ...")
b_new = np.load('../sample_npz/matrix_2037609/b.npy')
print(type(b_new), len(b_new), b_new)
print("Loading Successed!\n")