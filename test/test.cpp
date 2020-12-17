#define BOOST_TEST_MODULE project_test_module

#include <boost/test/unit_test.hpp>
#include "../src/matrix.h"

BOOST_AUTO_TEST_SUITE(project_test_suite)

	BOOST_AUTO_TEST_CASE(project_test_matrix_rect)
	{
		Matrix<int, -1> matrix; // бесконечная матрица int заполнена значениями -1
		assert(matrix.size() == 0); // все ячейки свободны

		auto a = matrix[0][0];
		assert(a == -1);
		assert(matrix.size() == 0);

		matrix[100][100] = 314;
		assert(matrix[100][100] == 314);
		assert(matrix.size() == 1);

		matrix[100][100] = -1;
		assert(matrix[100][100] == -1);
		assert(matrix.size() == 0);

		matrix(100, 100) = 314;
		assert(matrix(100, 100) == 314);
		assert(matrix.size() == 1);
	}

BOOST_AUTO_TEST_SUITE_END()
