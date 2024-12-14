class SparseMatrix:
    def __init__(self, rows, cols, dense_matrix=None):
        if not (1 <= rows <= 100 and 1 <= cols <= 100):
            raise ValueError("Matrix dimensions must be between 1 and 100.")
        self.rows = rows
        self.cols = cols
        self.data = self._to_sparse(dense_matrix) if dense_matrix else {}

    def _to_sparse(self, dense_matrix):
        """Converts a dense matrix into sparse format (row-wise storage)."""
        sparse_data = {}
        for i, row in enumerate(dense_matrix, start=1):
            for j, value in enumerate(row, start=1):
                if value != 0:
                    sparse_data[(i, j)] = value
        return sparse_data

    def trace(self):
        """Calculates the trace (sum of diagonal elements) of the matrix."""
        if self.rows != self.cols:
            raise ValueError("Trace is defined only for square matrices.")
        return sum(self.data.get((i, i), 0) for i in range(1, self.rows + 1))

    def get_element(self, row, col):
        """Returns the element at position (row, col) (indices starting from 1)."""
        if not (1 <= row <= self.rows and 1 <= col <= self.cols):
            raise IndexError("Row and column indices are out of bounds.")
        return self.data.get((row, col), 0)

    def __str__(self):
        """Displays the full matrix in dense format."""
        dense_matrix = [[0] * self.cols for _ in range(self.rows)]
        for (i, j), value in self.data.items():
            dense_matrix[i - 1][j - 1] = value
        return "\n".join(" ".join(map(str, row)) for row in dense_matrix)


def main():
    with open(r"D:\\lab-1-Linal\\polygon\\lab1-linal\\linal.txt", "r", encoding="utf-8") as file:
        lines = file.readlines()

    dimensions = list(map(int, lines[1].split()))
    dense_matrix = [list(map(float, lines[i + 2].split())) for i in range(dimensions[0])]

    matrix = SparseMatrix(dimensions[0], dimensions[1], dense_matrix)

    print("Trace:", matrix.trace())
    print("Element (2, 3):", matrix.get_element(2, 3))


if __name__ == "__main__":
    main()