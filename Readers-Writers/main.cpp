#include <mpi.h>
#include <iostream>

using std::cout;

struct Info {
  int procRank;
  int need;
  int access;
};

const int WRITE   = 1;
const int W_DONE  = 2;
const int READ    = 3;
const int R_DONE  = 4;
const int STOP    = 5;

int main(int argc, char** argv)
{
  int procNum, procRank;
  Info info;
  info.access = false;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Status status;

  // manager
  if (procRank == 0) {
    int countWriters = 0, countReaders = 0;
    int done = 0;

    while (true) {
      // receive msg
      MPI_Recv(&info, 3, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

      if (info.need == READ || info.need == WRITE)
        if (countWriters == 0 && countReaders == 0)
            info.access = true;
        else
            info.access = false;
      
      if (info.access == true)
        switch (info.need)
        {
          case WRITE:
            countWriters++;            
            break;

          case W_DONE:
            countWriters--;
            done++;
            break;

          case READ:
            countReaders++;
            break;

          case R_DONE:
            countReaders--;
            done++;
            break;

          default:
            break;
        }

      // send answer
      MPI_Ssend(&info, 3, MPI_INT, info.procRank, 0, MPI_COMM_WORLD);

      if (done == procNum - 1)
        break;
    }
  } else {
    // writers
    if (procRank % 2 == 0)
      while (true) {
        info.procRank = procRank;
        info.need = WRITE;

        // send request
        MPI_Ssend(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // wait answer
        MPI_Recv(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        if (info.access == true) {
          // do something cool
          cout << "Writer " << procRank << " get access!\n";

          // msg "done"
          cout << "Writer " << procRank << " done!\n";
          info.need = W_DONE;
          MPI_Ssend(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
          MPI_Recv(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
          break;
        }
      }

    // readers
    if (procRank % 2 != 0)
      while (true) {
        info.procRank = procRank;
        info.need = READ;
        // send request
        MPI_Ssend(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // wait answer
        MPI_Recv(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        if (info.access == true) {
          // do something cool
          cout << "Reader " << procRank << " read!\n";

          // msg "done"
          cout << "Reader " << procRank << " done!\n";
          info.need = R_DONE;
          MPI_Ssend(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD);
          MPI_Recv(&info, 3, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
          break;
        }
      }
  }

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();
  return 0;
}
