#include <iostream>
#include <omp.h>

using namespace std;

void MatrVecMult(double * A, double * b, double * c, int N) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            c[i] += A[i*N + j] * b[j];
    }
}
