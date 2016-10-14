#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
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
	int ProcNum, ProcRank;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);

	if (ProcRank == 0) {
		if (!checkArguments(argc, argv))
			exit(1);
	}

	unsigned int sum = 0, numOfChar = 0;;
	char symbol = argv[1][0];
	char* arr;

	double start, stop;

	string wait = "Wait";

	if (ProcRank == 0) {
		ifstream file(argv[2]);
		
		cout << wait;
		int i = 0;

		while (!file.eof()) {
			if (i++ % 1000000 == 0)
				if (wait.length() == 7) {
					wait = "Wait";
				} else {
					system("cls");
					wait += ".";
					cout << wait;
				}

			numOfChar++;
			file.get();
		}

		file.close();
	}

	MPI_Bcast(&numOfChar, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

	arr = new char[numOfChar];

	if (ProcRank == 0) {
		ifstream file(argv[2]);

		for (int i = 0; i < numOfChar; i++) {
			if (i % 1000000 == 0)
				if (wait.length() == 7) { 
					wait = "Wait";
				} else {
					system("cls");
					wait += ".";
					cout << wait;
				}

			arr[i] = file.get();
		}

		file.close();

		system("cls");
		cout << "number of char - " << numOfChar << "\n";
	}

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
			<< "Time for search- " << stop - start << " sec\n";
	}

	MPI_Finalize();
	delete arr;
	return 0;
}
