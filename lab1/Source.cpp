#include <iostream>
#include <fstream>
#include <iomanip>
#include "mpi.h"

using std::ifstream;
using std::cout;
using std::string;

void userMenu(const char* appname, const char* message) {
	cout << message << "\n"
		<< "Please, provide arguments in the following format:\n\n"

		<< "$ <name of programm> <symbol you want to find> <name of file>\n\n";
}

bool checkArguments(int argc, char** argv) {
	if (argc != 3) {
		userMenu(argv[0], "Wrong number of arguments!");
		return false;

	} else {
		if (argv[1][1] != 0) {
			userMenu(argv[0], "You must enter only one symbol!");
			return false;

		} else {
			ifstream checkFile(argv[2]);
			if (!checkFile.is_open()) {
				userMenu(argv[0], "Wrong name of file!");
				return false;
			}
		}
	}

	return true;
}

int main(int argc, char** argv) {
	int procNum, procRank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

	if (procRank == 0) {
		if (!checkArguments(argc, argv))
			exit(1);
	}

	unsigned int sum = 0, numOfChar = 0;;
	char symbol = argv[1][0];
	char* arr;

	double start, stop;

	if (procRank == 0) {
		ifstream file(argv[2]);

		while (!file.eof()) {
			file.get();
			numOfChar++;	
		}

		numOfChar--;

		file.close();
	}

	MPI_Bcast(&numOfChar, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

	arr = new char[numOfChar];

	if (procRank == 0) {
		ifstream file(argv[2]);

		for (int i = 0; i < numOfChar; i++)
			arr[i] = file.get();
		
		if (numOfChar == 6) {
			cout << "String: ";
			for (int i = 0; i < numOfChar; i++)
				cout << arr[i];
			cout << "\n";
		}

		file.close();

		cout << "Number of char: " << numOfChar << "\n";
	}

	MPI_Bcast(arr, numOfChar, MPI_CHAR, 0, MPI_COMM_WORLD);

	if (procRank == 0)
		start = MPI_Wtime();

	int i1, i2;
	int k = numOfChar / procNum;
	int procCount = 0;

	i1 = (int)(k * procRank);
	i2 = (int)(k * (procRank + 1));

	if (procRank == procNum - 1)
		i2 = numOfChar;

	for (int j = i1; j < i2; j++)
		if (arr[j] == symbol)
			procCount++;

	MPI_Reduce(&procCount, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
	
	if (procRank == 0) {
		stop = MPI_Wtime();
		double result = (double)sum / (double)numOfChar * 100;

		cout << "Count: " << sum << "\n"
			<< "Result: " << std::fixed << std::setprecision(2) << result << " %\n"
			<< "Time for search: " <<  stop - start << " sec\n";
	}

	MPI_Finalize();
	delete arr;
	return 0;
}
