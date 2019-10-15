// Copyright 2019 Astafeva Irina

#include <iostream>
#include <cstdlib>
#include "astafeva_i_max_val_in_row.h"

int main() {
	int r = 5;
	int c = 5;
	
	std::vector<int> v = getRandomMatrix(r, c);

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			std::cout << v[i * c + j] << " ";
		}
		std::cout << std::endl;
	}

	std::vector<int> max = getMaxInRows(v, r, c);
	for (int i = 0; i < r; i++)
		std::cout << max[i] << " ";
	std::cout << std::endl;

	system("pause");

	return 0;
}
