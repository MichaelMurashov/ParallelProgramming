#include <iostream>
#include <fstream>
#include "mpi.h"

using std::ifstream;
using std::cout;

int main(int argc, char** argv) {
	int ProcNum, ProcRank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	int sum = 0, numOfChar = 0;;
	char symbol = argv[1][0];
	char* arr;

	double start, stop;

	// работаем в нулевом главном процессе
	if (ProcRank == 0) {
		ifstream file(argv[2]);
		
		// подсчитали символы
		while (!file.eof()) {
			numOfChar++;
			file.get();
		}

		file.close();
	}

	// расшарили переменную numOfChar
	MPI_Bcast(&numOfChar, 1, MPI_LONG, 0, MPI_COMM_WORLD);

	// выделили память в каждом процессе
	arr = new char[numOfChar];

	// снова работаем в главном процессе
	if (ProcRank == 0) {
		ifstream file(argv[2]);

		// заполнили память
		for (int i = 0; i < numOfChar; i++)
			arr[i] = file.get();

		file.close();

		cout << "num - " << numOfChar << "\n";
	}

	// расшарили массив
	MPI_Bcast(arr, numOfChar, MPI_CHAR, 0, MPI_COMM_WORLD);

	if (ProcRank == 0)
		start = MPI_Wtime();

	int i1, i2;
	int k = numOfChar / ProcNum;
	int procCount = 0;

	i1 = (int)(k * ProcRank);
	i2 = (int)(k * (ProcRank + 1));

	if (ProcRank == ProcNum - 1)
		i2 = numOfChar;

	for (int j = i1; j < i2; j++)
		if (arr[j] == symbol)
			procCount++;

	MPI_Reduce(&procCount, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (ProcRank == 0) {
		stop = MPI_Wtime();
		double result = (double)sum / (double)numOfChar * 100;

		cout << "Result: " << std::fixed << result << " %\n"
			<< "time - " << stop - start << " sec\n";
	}

	MPI_Finalize();
	delete arr;
	return 0;
}
