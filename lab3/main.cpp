#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <time.h>

using std::cout;

int const n = 1000;
int count = 0;
int timer, tin[n], fup[n];
bool used[n];
int mx[n][n] /*= {
  { 0,1,1,1,0,0,0,0,0,0,0,0 },
  { 1,0,0,0,1,1,0,0,0,0,0,0 },
  { 1,0,0,0,0,0,0,0,0,0,0,0 },
  { 1,0,0,0,0,0,1,1,0,0,0,0 },
  { 0,1,0,0,0,0,0,0,1,1,0,0 },
  { 0,1,0,0,0,0,0,0,0,0,0,0 },
  { 0,0,0,1,0,0,0,0,0,0,1,0 },
  { 0,0,0,1,0,0,0,0,0,0,0,1 },
  { 0,0,0,0,1,0,0,0,0,0,0,0 },
  { 0,0,0,0,1,0,0,0,0,0,0,0 },
  { 0,0,0,0,0,0,1,0,0,0,0,0 },
  { 0,0,0,0,0,0,0,1,0,0,0,0 }
}*/;

void dfs(int v, int p = -1) {
  used[v] = true;
  tin[v] = fup[v] = timer++;

  for (int i = 0; i < n; ++i) {
    if (mx[v][i] == 1) {
      int to = i;
      if (to == p)
        continue;

      if (used[to])
        fup[v] = std::min(fup[v], tin[to]);
      else {
        dfs(to, v);
        fup[v] = std::min(fup[v], fup[to]);

        if (fup[to] > tin[v])
          count++;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  srand(time(0));
  int procRank, procNum;
  double start, stop;
  timer = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) {
        int tmp = rand() % n;
        if (!tmp)
          mx[i][j] = 1;
      }

    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        if (mx[i][j])
          mx[j][i] = 1;

    /*for (int i = 0; i < n; ++i) {
      for (int j = 0; j < n; ++j)
        std::cout << mx[i][j];
      std::cout << "\n";
    }*/
  }

  MPI_Bcast(&mx, n*n, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < n; i++)
    used[i] = false;

  if (procRank == 0)
    start = MPI_Wtime();

  MPI_Barrier(MPI_COMM_WORLD);

  for (int i = procRank; i < n; i += procNum)
    if (!used[i])
      dfs(i);

  cout << procRank << ": " << count << "\n";

  MPI_Barrier(MPI_COMM_WORLD);

  if (procRank == 0) {
    stop = MPI_Wtime();
    cout << "time - " << stop - start << "\n";
  }

  MPI_Finalize();
  return 0;
}