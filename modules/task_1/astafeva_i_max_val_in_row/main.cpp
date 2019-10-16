// Copyright 2019 Astafeva Irina

#include <iostream>
#include <cstdlib>
#include <gtest-mpi-listener.hpp>
#include "astafeva_i_max_val_in_row.h"

int main(int argc, char** argv) {
	int r = 6;
	int c = 6;

	std::vector<int> v = getRandomMatrix(r, c);
	std::vector<int> max = getMaxInRows(v, r, c);

	MPI_Init(&argc, &argv);
	int rank, size;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	std::vector<int> maxPar = getMaxInRowsPar(v, r, c);

	MPI_Finalize();
	if (rank == 0) {
		std::cout << size << std::endl << std::endl;
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				std::cout << v[i * c + j] << " ";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		for (int i = 0; i < r; i++)
			std::cout << max[i] << " ";
		std::cout << std::endl;
		std::cout << std::endl;

		for (int i = 0; i < r; i++)
			std::cout << max[i] << " ";
		std::cout << std::endl;
		std::cout << std::endl;
	}
	return 0;
}
