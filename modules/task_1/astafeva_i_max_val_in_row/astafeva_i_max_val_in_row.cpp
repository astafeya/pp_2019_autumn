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

std::vector<int> getMaxInRowsPar(const std::vector<int> matrix, int rows, int columns) {
	if ((rows <= 0) || (columns <= 0)) throw - 1;
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int delta_rows = rows / size;
	const int rem_rows = rows % size;
	int start_row = delta_rows;
	if (rem_rows != 0) {
		start_row++;
	}

	if ((rank == 0)&&(size > 1)) {
		for (int proc = 1; proc < size; proc++) {
			if (proc < rem_rows) {
				MPI_Send(&matrix[start_row * columns], (delta_rows + 1) * columns, MPI_INT, proc, 0, MPI_COMM_WORLD);
				start_row += delta_rows;
				start_row++;
			}
			else {
				MPI_Send(&matrix[start_row * columns], delta_rows * columns, MPI_INT, proc, 0, MPI_COMM_WORLD);
				start_row += delta_rows;
			}
		}
	}

	std::vector<int> local;
	int local_rows;
	if (rank == 0) {
		if (rem_rows != 0) {
			local_rows = delta_rows + 1;
		}
		else {
			local_rows = delta_rows;
		}
		local.resize(local_rows * columns);
		local = std::vector<int>(&matrix[0], &matrix[0] + local_rows * columns);
	}
	else {
		if (rank < rem_rows) {
			local_rows = delta_rows + 1;
		}
		else {
			local_rows = delta_rows;
		}
		local.resize(local_rows * columns);
		MPI_Status status;
		MPI_Recv(&local[0], local_rows * columns, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
	}

	std::vector<int> global_result(rows);
	std::vector<int> local_result = getMaxInRows(local, local_rows, columns);

	MPI_Gather(&local_result[0], local_rows, MPI_INT, &global_result[0], local_rows, MPI_INT, 0, MPI_COMM_WORLD);
	return global_result;
}
