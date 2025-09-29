#include <iostream>
#include <array>

template <typename T, size_t Rows, size_t Cols>
struct Matrix {
    std::array<T, Rows * Cols> data;

    T& operator[](size_t i, size_t j) {
        return data[i * Cols + j];
    }

    const T& operator[](size_t i, size_t j) const {
        return data[i * Cols + j];
    }
};

int main() {
    Matrix<int, 3, 3> mat = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::cout << mat[1, 2] << "\n";  // 输出 6

    return 0;
}