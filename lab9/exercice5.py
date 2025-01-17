import numpy as np

# Path to the file
file_path = r"D:\program\C++program\lab9algo\polygon\lab9\test5.txt"

# Reading data from the file
data = np.loadtxt(file_path, dtype=int)

# Function to count numbers divisible by 3
def count_divisible_by_three(arr):
    return np.sum(arr % 3 == 0, axis=1)

# Counting and printing results
results = count_divisible_by_three(data)
print("Number of integers divisible by 3 in each line:")
print(results)
