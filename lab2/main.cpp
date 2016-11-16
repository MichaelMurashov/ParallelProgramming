#include <mpi.h>
#include <iostream>

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
  int procNum, procRank;
  int flag = true;
  int* count_write;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Status status;

  count_write = new int[procNum];

  if (procRank % 2 == 0) {
    //writer
    cout << "Writer " << procRank << " come" << endl;

    for (int i = 0; i < procNum; i++)
      if (count_write[i] == 1) {
        cout << "Writer " << procRank << " wait" << endl;
        MPI_Recv(count_write, procNum, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        flag = false;
      }

    if (flag == true) {
      count_write[procRank] = 1;
      for (int i = 0; i < procNum; i++)
        MPI_Send(count_write, procNum, MPI_INT, i, 0, MPI_COMM_WORLD);

      cout << "Writer " << procRank << " write" << endl;
      cout << "Writer " << procRank << " gone" << endl;

      count_write[procRank] = 0;
      for (int i = 0; i < procNum; i++)
        MPI_Send(count_write, procNum, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

  } else {
    //reader
    cout << "Reader " << procRank << " come" << endl;

    for (int i = 0; i < procNum; i++)
      if (count_write[i] == 1) {
        cout << "Reader " << procRank << " wait" << endl;
        MPI_Recv(count_write, procNum, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
      }

    cout << "Reader " << procRank << " read" << endl;
    cout << "Reader " << procRank << " gone" << endl;
  }

  MPI_Finalize();
  return 0;
}
