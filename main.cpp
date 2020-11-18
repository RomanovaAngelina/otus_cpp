#include <iostream>
#include <tuple>

#include "matrix.h"

int main(int, char*[])
{
    Matrix<int, 0> matrix;
    for(size_t i = 0; i <= 9; ++i) {
        matrix[i][i]     = i;
        matrix[i][9 - i] = 9 - i;
    }

    for (size_t x = 1; x <= 8; ++x) {
        for(size_t y = 1; y <=8; ++y) {
            std::cout << matrix[x][y] << " ";
        }
        std::cout << std::endl;
    }

    std::cout << matrix.size() << std::endl;

    for (auto cell : matrix) {
        size_t row    = 0;
        size_t colomn = 0;
        int    value  = 0;
        std::tie(row, colomn, value) = cell;
        std::cout << "matrix[" << row << "][" << colomn << "] = " << value << std::endl;
    }

    ((matrix[100][100] = 314) = 0) = 217;
    std::cout << "matrix[100][100] = " << matrix[100][100] << std::endl;
    return 0;
}