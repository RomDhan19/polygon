import numpy as np

# Function to find the index of the n-th maximum distinct value
def nth_max_index(arr, n):
    # Get the unique values in descending order
    unique_vals = np.unique(arr)[::-1]
    
    # Check if n-th distinct maximum exists
    if n > len(unique_vals):
        return None  
    
   
    nth_max_value = unique_vals[n - 1]
    
    
    return np.where(arr == nth_max_value)[0][0] + 1


file_path = r"D:\program\C++program\lab9algo\polygon\lab9\test6.txt"


data = np.loadtxt(file_path, dtype=int)


n_values = data[:, -1] 
arrays = data[:, :-1]   

# Applying the function to each row (no explicit loop)
results = np.array([nth_max_index(arr, n) for arr, n in zip(arrays, n_values)])


print(results)
