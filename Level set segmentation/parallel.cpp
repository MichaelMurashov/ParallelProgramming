#include <iostream>
#include <ctime>

#include <mpi.h>
#include <omp.h>

#include <tbb/task_scheduler_init.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>


void openMP(const int NUM_OF_THREADS) {
	const int size = 200000000;
	int* src = new int[size];

	omp_set_num_threads(NUM_OF_THREADS);

	const clock_t start = clock();

	#pragma omp parallel for
	for (int i = 0; i < size; i++) {
		src[i] = rand() % 255;
	}

	int threshold = rand() % 255;
	int range = 10;

	#pragma omp parallel for
	for (int i = 0; i < size; i++) {
		int intensity = src[i];
		if ((threshold - range) > intensity || intensity > (threshold + range))
			src[i] = -1;
	}

	const clock_t finish = clock();

	double time = (float)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "Num of threads: " << NUM_OF_THREADS << std::endl
		<< "Time - " << time << std::endl << std::endl;
}

void tbb_1(const int NUM_OF_THREADS) {
	const int size = 200000000;
	int* src = new int[size];

	tbb::task_scheduler_init init(NUM_OF_THREADS);

	const clock_t start = clock();

	tbb::parallel_for(
		tbb::blocked_range<int>(0, size),
		[&](tbb::blocked_range<int> r) {
			for (int i = r.begin(); i < r.end(); ++i) {
				src[i] = rand() % 255;
			}
		}
	);

	int threshold = rand() % 255;
	int range = 10;

	tbb::parallel_for(
		tbb::blocked_range<int>(0, size),
		[&](tbb::blocked_range<int> r) {
			for (int i = r.begin(); i < r.end(); ++i) {
				int intensity = src[i];
				if ((threshold - range) > intensity || intensity > (threshold + range))
					src[i] = -1;
			}
		}
	);

	const clock_t finish = clock();

	double time = (float)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "Num of threads: " << NUM_OF_THREADS << std::endl
		<< "Time - " << time << std::endl << std::endl;
}

void mpi_openMP(int argc, char* argv[], const int NUM_OF_THREADS) {
	const int size = 200000000;
	int* src = new int[size];
	int* result = new int[size];

	clock_t start, finish;
	int threshold, range = 10;
	
	int procNum, procRank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

	omp_set_num_threads(NUM_OF_THREADS);

	if (procRank == 0)
		threshold = rand() % 255;
	MPI_Bcast(&threshold, 1, MPI_INT, 0, MPI_COMM_WORLD);

	int k = size / procNum;
	int i1 = (int)(k * procRank);
	int i2 = (int)(k * (procRank + 1));
	if (procRank == procNum - 1)
		i2 = size;

	MPI_Barrier(MPI_COMM_WORLD);

	if (procRank == 0)
		start = clock();

	#pragma omp parallel for
	for (int i = i1; i < i2; i++) {
		src[i] = rand() % 255;
		int intensity = src[i];
		if ((threshold - range) > intensity || intensity > (threshold + range))
			src[i] = -1;
	}

	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Gather(src, k, MPI_INT, result, k, MPI_INT, 0, MPI_COMM_WORLD);

	if (procRank == 0) {
		finish = clock();
		double time = (float)(finish - start) / CLOCKS_PER_SEC;
		std::cout << "Num of process: " << procNum << std::endl
			<< "Num of threads: " << NUM_OF_THREADS << std::endl
			<< "Time - " << time << std::endl << std::endl;
	}

	MPI_Finalize();
}

int main(int argc, char* argv[]) {
	int NUM_OF_THREADS = 2;

	//openMP(NUM_OF_THREADS);
	//tbb_1(NUM_OF_THREADS);
	mpi_openMP(argc, argv, NUM_OF_THREADS);

	//system("pause");
	return 0;
}
