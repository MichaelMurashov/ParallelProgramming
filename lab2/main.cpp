#include <mpi.h>
#include <iostream>

using std::cout;

struct Info {
  int procRank;
  int need;
  int access;
};

const int WRITE   = 1;
const int W_DONE  = 3;
const int STOP    = 5;

int main(int argc, char** argv)
{
  int procNum, procRank;
  Info info;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Status status;

  // manager
  if (procRank == 0) {
    int countWriters = 0;
    int done = 0;
    int* flags = new int[procNum];
    for (int i = 0; i < procNum; i++)
      flags[i] = false;

    while (true) {
      // receive msg
      MPI_Recv(&info, 3, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

      if (countWriters == 0)
        if (flags[info.procRank] == false) {
          flags[info.procRank] = true;
          info.access = true;
        } else {
          info.access = false;
        }

      switch (info.need)
      {
      case WRITE:
        countWriters++;
        break;

      case W_DONE:
        countWriters--;
        done++;
        break;

      default:
        break;
      }

      if (done == procNum / 2) {
        info.access = STOP;
        cout << "hello ";
        for (int i = 0; i < procNum; i++)
          MPI_Send(&info, 3, MPI_INT, i, 0, MPI_COMM_WORLD);
        break;
      }

      // send answer
      MPI_Send(&info, 3, MPI_INT, info.procRank, 0, MPI_COMM_WORLD);
    }
  }

  // writers
  if (procRank % 2 == 0)
    while (true) {
      info.procRank = procRank;
      info.need = WRITE;
      // send request
      MPI_Send(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
      // wait answer
      MPI_Recv(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

      if (info.access == STOP)
        break;

      if (info.access == true) {
        // do something cool
        cout << "Writer " << procRank << " get access!\n";

        // msg "done"
        cout << "Writer " << procRank << " done!\n";
        info.need = W_DONE;
        MPI_Send(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }

  // readers
  if (procRank % 2 != 0) 
    while (true) {
      info.procRank = procRank;
      // send request
      MPI_Send(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
      // wait answer
      MPI_Recv(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

      if (info.access == STOP) {
        break;
      }

      if (info.access == true) {
        // do something cool
        cout << "Reader " << procRank << " read!\n";

        // msg "done"
        MPI_Send(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
      }
    }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
