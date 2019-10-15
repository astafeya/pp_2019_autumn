// Copyright 2019 Astafeva Irina

#include <mpi.h>
#include <ctime>
#include <algorithm>
#include "astafeva_i_max_val_in_row.h"

std::vector<int> getRandomMatrix(int rows, int columns) {
	srand(time(NULL));
	const size_t r = rows;
	const size_t c = columns;
	const size_t size = r * c;
	std::vector<int> matrix(size);
	for (size_t i = 0; i < size; i++) {
		matrix[i] = rand() % 101;
	}
	return matrix;
}

std::vector<int> getMaxInRows(const std::vector<int> matrix, int rows, int columns) {
	if ((rows <= 0) || (columns <= 0)) throw - 1;
	const size_t r = rows;
	const size_t c = columns;
	std::vector<int> maxs(r);
	int j = 0;
	for (size_t i = 0; i < r * c; i += c) {
		int maxInRow = *std::max_element(&matrix[i], &matrix[i+c]);
		maxs[j] = maxInRow;
		j++;
	}
	return maxs;
}
