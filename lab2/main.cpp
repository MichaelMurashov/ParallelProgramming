#include <mpi.h>
#include "semaphore.h"

int readCount = 0;
Semaphore *readSem, *access;

void writer() {
  while (true) {
    access->wait();

    // write

    access->signal();
  }
}

void reader() {
  while (true) {
    readSem->wait();
    readCount++;

    if (readCount == 1)
      access->wait();

    readSem->signal();

    // read

    readSem->wait();
    readCount--;

    if (readCount == 0)
      access->signal();

    readSem->wait();
  }
}

int main(int argc, char** argv) {
  int procNum, procRank;
  double start, stop;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    access = new Semaphore(procNum);
    readSem = new Semaphore(procNum);
  }



  MPI_Finalize();
	return 0;
}
