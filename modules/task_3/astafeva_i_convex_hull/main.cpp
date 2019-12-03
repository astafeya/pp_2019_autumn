// Copyright 2019 Astafeva Irina

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "..\..\..\modules\task_3\astafeva_i_convex_hull\convex_hull.h"

TEST(Convex_Hull, name) {
    int size, rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);    
    int height = 10; //5
    int width = 5;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    /*image[0][0] = 0; image[0][1] = 0; image[0][2] = 0; image[0][3] = 0; image[0][4] = 0;
    image[1][0] = 0; image[1][1] = 1; image[1][2] = 1; image[1][3] = 1; image[1][4] = 0;
    image[2][0] = 0; image[2][1] = 0; image[2][2] = 0; image[2][3] = 1; image[2][4] = 0;
    image[3][0] = 0; image[3][1] = 1; image[3][2] = 1; image[3][3] = 1; image[3][4] = 0;
    image[4][0] = 0; image[4][1] = 0; image[4][2] = 1; image[4][3] = 0; image[4][4] = 0;*/

    image[0][0] = 1; image[0][1] = 0; image[0][2] = 0; image[0][3] = 0; image[0][4] = 0;
    image[1][0] = 1; image[1][1] = 0; image[1][2] = 1; image[1][3] = 1; image[1][4] = 1;
    image[2][0] = 0; image[2][1] = 0; image[2][2] = 0; image[2][3] = 0; image[2][4] = 1;
    image[3][0] = 1; image[3][1] = 0; image[3][2] = 0; image[3][3] = 1; image[3][4] = 1;
    image[4][0] = 1; image[4][1] = 1; image[4][2] = 0; image[4][3] = 0; image[4][4] = 1;
    image[5][0] = 1; image[5][1] = 0; image[5][2] = 0; image[5][3] = 0; image[5][4] = 0;
    image[6][0] = 1; image[6][1] = 0; image[6][2] = 1; image[6][3] = 1; image[6][4] = 1;
    image[7][0] = 0; image[7][1] = 0; image[7][2] = 0; image[7][3] = 0; image[7][4] = 1;
    image[8][0] = 1; image[8][1] = 0; image[8][2] = 0; image[8][3] = 1; image[8][4] = 1;
    image[9][0] = 1; image[9][1] = 1; image[9][2] = 0; image[9][3] = 0; image[9][4] = 1;

    if (rank == 0) {
        printImage(image, height, width);
        std::cout << std::endl;
    }

    int ** result = buildComponentConvexHull(image, height, width);
    
    if (rank == 0) printImage(result, height, width);
    //ASSERT_FAILURE();
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
