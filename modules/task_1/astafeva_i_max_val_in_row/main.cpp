// Copyright 2019 Astafeva Irina

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_1/astafeva_i_max_val_in_row/max_val_in_row.h"

TEST(Get_Random_Matrix, Cant_Create_Matrix_With_Negative_Parametrs) {
    ASSERT_ANY_THROW(std::vector<int> matrix = getRandomMatrix(-2, 10));
    ASSERT_ANY_THROW(std::vector<int> matrix = getRandomMatrix(5, -8));
}

TEST(Get_Max_In_Rows_Par, Rows_Aliquot_Size) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rows = 2 * size;
    int columns = 5;

    std::vector<int> matrix = getRandomMatrix(rows, columns);

    std::vector<int> result1 = getMaxInRowsPar(matrix, rows, columns);

    if (rank == 0) {
        std::vector<int> result2 = getMaxInRows(matrix, rows, columns);
        ASSERT_EQ(result1, result2);
    }
}

TEST(Get_Max_In_Rows_Par, Rows_Not_Aliquot_Size) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rows = 2 * size + size / 2;
    int columns = 5;

    std::vector<int> matrix = getRandomMatrix(rows, columns);

    std::vector<int> result1 = getMaxInRowsPar(matrix, rows, columns);

    if (rank == 0) {
        std::vector<int> result2 = getMaxInRows(matrix, rows, columns);
        ASSERT_EQ(result1, result2);
    }
}

TEST(Get_Max_In_Rows_Par, Cant_Work_With_Empty_Matrix) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int rows = 0;
    int columns = 0;

    std::vector<int> matrix = getRandomMatrix(0, 0);

    ASSERT_ANY_THROW(std::vector<int> result1 = getMaxInRowsPar(matrix, rows, columns));

    if (rank == 0) {
        ASSERT_ANY_THROW(std::vector<int> result2 = getMaxInRows(matrix, rows, columns));
    }
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
