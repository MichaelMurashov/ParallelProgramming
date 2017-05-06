#include <iostream>
#include <iomanip>
#include <omp.h>

using namespace std;

int main(int argc, char* argv[]) {
    int n = atoi(argv[1]);
    int numOfThreads = atoi(argv[2]);

    omp_set_num_threads(numOfThreads);

    double sum = 0;
    double x, dx = 1.0 / n;

    #pragma omp parallel for private(x), reduction(+:sum)
    for (int i = 0; i < n; i++) {
        x = (i + 0.5) * dx;
        sum += 4.0 / (1.0 + x*x);
    }

    double result = sum * dx;

    cout << setprecision(16) << result << endl;

    return 0;
}
