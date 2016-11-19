#include <mpi.h>
#include <iostream>

using std::cout;

int main(int argc, char** argv)
{
  int procNum, procRank;
  int flag = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  MPI_Status status;

  // выделим в отдельный коммуникатор первую половину процессов
  /*MPI_Group group_world, writers;
  MPI_Comm_group(MPI_COMM_WORLD, &group_world);
  int size = procNum / 2;
  int* members = new int[size];
  for (int i = 0; i < size; i++)
    members[i] = 2 * i;
  MPI_Group_incl(group_world, size, members, &writers);
  MPI_Comm writes_comm;
  MPI_Comm_create(MPI_COMM_WORLD, writers, &writes_comm);*/

  MPI_Barrier(MPI_COMM_WORLD);

  // writers
  if (procRank == 0)
    cout << "Writer " << procRank << " wr!\n";

  MPI_Barrier(MPI_COMM_WORLD);

  if (procRank == 2)
    cout << "Writer " << procRank << " wr!\n";

  MPI_Barrier(MPI_COMM_WORLD);

  // readers
  if (procRank % 2 != 0)
    cout << "Reader " << procRank << " read\n";

  MPI_Finalize();
  return 0;
}
