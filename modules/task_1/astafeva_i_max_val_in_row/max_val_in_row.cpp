// Copyright 2019 Astafeva Irina

#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include "../../../modules/task_1/astafeva_i_max_val_in_row/max_val_in_row.h"

std::vector<int> getRandomMatrix(int rows, int columns) {
    if ((rows < 0) || (columns < 0)) throw - 1;
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    const size_t r = rows;
    const size_t c = columns;
    const size_t size = r * c;
    std::vector<int> matrix(size);
    for (size_t i = 0; i < size; i++) {
        matrix[i] = gen() % 101;
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
    if (rows == 1) {
        std::vector<int> result;
        result.push_back(*std::max_element(&matrix[0], &matrix[columns]));
        return result;
    }
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    size_t rows_t = rows;
    size_t columns_t = columns;
    size_t s_t = size;
    std::vector<int> recvcounts(s_t);
    std::vector<int> displs(s_t);
    int s = size;
    if (size > rows) {
        s = rows;
        s_t = rows;
    }
    const int delta_rows = rows / s;
    const int rem_rows = rows % s;
    int start_row = delta_rows;
    size_t start_row_t = delta_rows;
    if (rem_rows != 0) {
        start_row++;
        start_row_t++;
    }
    recvcounts[0] = start_row;
    displs[0] = 0;
    if ((rank == 0)&&(size > 1)) {
        size_t proc_t = 1;
        for (int proc = 1; proc < s; proc++) {
            if (proc < rem_rows) {
                MPI_Send(&matrix[start_row_t * columns_t], (delta_rows + 1) * columns, MPI_INT, proc, 0, MPI_COMM_WORLD);
                recvcounts[proc_t] = delta_rows + 1;
                displs[proc_t] = start_row;
                proc_t++;
                start_row += delta_rows;
                start_row++;
                start_row_t = start_row;
            } else {
                MPI_Send(&matrix[start_row_t * columns_t], delta_rows * columns, MPI_INT, proc, 0, MPI_COMM_WORLD);
                recvcounts[proc_t] = delta_rows;
                displs[proc_t] = start_row;
                proc_t++;
                start_row += delta_rows;
                start_row_t = start_row;
            }
        }
        if (s != size) {
            size_t proc_t = s_t;
            for (int proc = s; proc < size; proc++) {
                recvcounts[proc_t] = 0;
                displs[proc_t] = start_row - 1;
                proc_t++;
            }
        }
    }

    std::vector<int> local;
    int local_rows = 0;
    if (rank == 0) {
        if (rem_rows != 0) {
            local_rows = delta_rows + 1;
        } else {
            local_rows = delta_rows;
        }
        size_t r_t = local_rows * columns;
        local.resize(r_t);
        local = std::vector<int>(&matrix[0], &matrix[0] + local_rows * columns);
    } else {
        if (rank < rem_rows) {
            local_rows = delta_rows + 1;
        } else {
            local_rows = delta_rows;
        }
        size_t r_t = local_rows * columns;
        local.resize(r_t);
        if (rank < s) {
            MPI_Status status;
            MPI_Recv(&local[0], local_rows * columns, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        }
    }
    std::vector<int> global_result(rows_t);
    std::vector<int> local_result = getMaxInRows(local, local_rows, columns);

    MPI_Gatherv(&local_result[0], local_rows, MPI_INT, &global_result[0],
        &recvcounts[0], &displs[0], MPI_INT, 0, MPI_COMM_WORLD);
    return global_result;
}
