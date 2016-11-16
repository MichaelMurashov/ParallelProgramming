#include <mpi.h>
#include <iostream>

using std::cout;

int main(int argc, char** argv) {
  const int size = 10;
  int procNum, procRank;
  int flag = true;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Request requestFlag, requestData;
  MPI_Status status;

  int* data = new int[size];
  for (int i = 0; i < size; i++)
    data[i] = i;

  if (procRank == 0) {
    cout << "data: ";
    for (int i = 0; i < size; i++)
      cout << data[i] << ' ';
    cout << '\n';
  }

  MPI_Barrier(MPI_COMM_WORLD);

  // writer
  if (procRank % 2 != 0)
    while (true) {
      if (flag == true) {
        flag = false;
        MPI_Isend(&flag, 1, MPI_INT, procRank, 0, MPI_COMM_WORLD, &requestFlag);

        MPI_Irecv(data, size, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &requestData);
        MPI_Wait(&requestData, &status);
        cout << "write " << procRank << ": ";
        for (int i = 0; i < size; i++) {
          data[i] += 10;
          cout << data[i] << ' ';
        }
        cout << '\n';
        MPI_Isend(data, size, MPI_CHAR, procRank, 1, MPI_COMM_WORLD, &requestData);

        flag = true;
        MPI_Isend(&flag, 1, MPI_INT, procRank, 0, MPI_COMM_WORLD, &requestFlag);
        break;
      }

      MPI_Irecv(&flag, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &requestFlag);
      MPI_Wait(&requestFlag, &status);
    }

  MPI_Barrier(MPI_COMM_WORLD);

  // reader
  if (procRank % 2 == 0) {
    cout << "read " << procRank << ": ";
    for (int i = 0; i < size; i++)
      cout << data[i] << ' ';
    cout << "\n";
  }

  MPI_Finalize();
	return 0;
}