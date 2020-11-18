#include "matrix.h"
#include <list>
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(MatrixTests, defaultElements)
{
{
Matrix<int, 5> matrix;
ASSERT_EQ(5, matrix[0][0]);
ASSERT_EQ(5, matrix[54][298]);
}
{
Matrix<int, -1> matrix;
ASSERT_EQ(-1, matrix[33][22]);
}
}

TEST(MatrixTests, insertingElements)
{
size_t nExpectedSize = 0;
Matrix<int, 0> matrix;
for (size_t x = 0; x < 500; ++x) {
for(size_t y = 0; y < 500; ++y) {
// skipping rows and coloms, that divisiable by 3
if (x % 3 && y % 3) {
matrix[x][y] = (x << 8) + y;
++nExpectedSize;
}
}
}

ASSERT_EQ(nExpectedSize, matrix.size());

for (size_t y = 0; y < 500; ++y) {
for(size_t x = 0; x < 500; ++x) {
// skipping rows and coloms, that divisiable by 10
if (x % 3 && y % 3) {
ASSERT_EQ((x << 8) + y, matrix[x][y]);
} else {
ASSERT_EQ(0, matrix[x][y]);
}
}
}
}

TEST(MatrixTests, insertingDefaultValue)
{
const int defaultValue = 5;

Matrix<int, defaultValue> matrix;
EXPECT_EQ(0u, matrix.size());

matrix[5][5] = 10;
EXPECT_EQ(1u, matrix.size());
EXPECT_EQ(10, matrix[5][5]);

matrix[5][5] = defaultValue;
EXPECT_EQ(defaultValue, matrix[5][5]);
EXPECT_EQ(0u, matrix.size());
}

TEST(Matrix, iteratorTest)
{
Matrix<int, 0> matrixA;

for (size_t x = 0; x < 20; ++x) {
for(size_t y = 0; y < 20; ++y) {
// skipping rows and coloms, that divisiable by 3
if (x % 3 && y % 3) {
matrixA[x][y] = (x << 8) + y;
}
}
}

Matrix<int, 0> matrixB = matrixA;

for(auto c: matrixA)
{
size_t x;
size_t y;
int    v;
std::tie(x, y, v) = c;
EXPECT_EQ(matrixB[x][y], v);
matrixB[x][y] = 0;
}
ASSERT_EQ(0u, matrixB.size());
}