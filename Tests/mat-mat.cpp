#include <iostream>
#include <omp.h>

using namespace std;

void MatrMatrMult(double * A, double * B, double * C, int N) {
    int i, j, k;  // Loop variables
    int NestedThreadsNum = 2;

    omp_set_nested(true);
    omp_set_num_threads(NestedThreadsNum);

    #pragma omp parallel for private (j, k)
    for (i = 0; i < N; i++)
        #pragma omp parallel for private (k)
        for (j = 0; j < N; j++)
            for (k = 0; k < N; k++)
                C[i*N + j] += A[i*N + k] * B[k*N + j];
}
