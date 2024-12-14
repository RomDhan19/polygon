class SparseMatrix:
    def __init__(self, rows, cols, dense_matrix=None):
        if not (1 <= rows <= 100 and 1 <= cols <= 100):
            raise ValueError("Matrix dimensions must be between 1 and 100.")
        self.rows = rows
        self.cols = cols
        self.data = self._to_sparse(dense_matrix) if dense_matrix else {}

    def _to_sparse(self, dense_matrix):
        sparse_data = {}
        for i, row in enumerate(dense_matrix, start=1):
            for j, value in enumerate(row, start=1):
                if value != 0:
                    sparse_data[(i, j)] = value
        return sparse_data

    def _convert_back(self):
        dense_matrix = [[0] * self.cols for _ in range(self.rows)]
        for (i, j), value in self.data.items():
            dense_matrix[i - 1][j - 1] = value
        return dense_matrix


def determinant(matrix):
    if matrix.rows != matrix.cols:
        raise ValueError("Determinant is defined only for square matrices.")
    dense_matrix = matrix._convert_back()
    return _determinant_recursive(dense_matrix)


def _determinant_recursive(matrix):
    n = len(matrix)
    if n == 1:
        return matrix[0][0]
    if n == 2:
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0]
    det = 0
    for col in range(n):
        minor = [row[:col] + row[col + 1:] for row in matrix[1:]]
        det += ((-1) ** col) * matrix[0][col] * _determinant_recursive(minor)
    return det


def is_invertible(matrix):
    return determinant(matrix) != 0


def main():
    with open(r"D:\\lab-1-Linal\\polygon\\lab1-linal\\linal.txt", "r", encoding="utf-8") as file:
        lines = file.readlines()

    # Skip lines starting with '#' and empty lines
    lines = [line for line in lines if line.strip() and not line.startswith('#')]

    # Debugging: Print the lines after filtering
    print("Filtered lines:", lines)

    # Read the first matrix (Invertible)
    dimensions1 = list(map(int, lines[0].split()))  # Matrix 1 dimensions (first line after filtering)
    print("Matrix 1 Dimensions:", dimensions1)  # Debugging line

    dense_matrix1 = [list(map(float, lines[i + 1].split())) for i in range(dimensions1[0])]  # Matrix 1 data
    print("Matrix 1 Data:", dense_matrix1)  # Debugging line

    matrix1 = SparseMatrix(dimensions1[0], dimensions1[1], dense_matrix1)
    det1 = determinant(matrix1)
    print(det1)
    print("yes" if det1 != 0 else "no")

    # Read the second matrix (Non-Invertible)
    dimensions2 = list(map(int, lines[4].split()))  # Matrix 2 dimensions (after Matrix 1, starting from line 5)
    print("Matrix 2 Dimensions:", dimensions2)  # Debugging line

    dense_matrix2 = [list(map(float, lines[i + 5].split())) for i in range(dimensions2[0])]  # Matrix 2 data
    print("Matrix 2 Data:", dense_matrix2)  # Debugging line

    matrix2 = SparseMatrix(dimensions2[0], dimensions2[1], dense_matrix2)
    det2 = determinant(matrix2)
    print(det2)
    print("yes" if det2 != 0 else "no")


if __name__ == "__main__":
    main()