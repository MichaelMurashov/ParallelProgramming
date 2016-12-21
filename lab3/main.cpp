#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <time.h>
#include <fstream>

using std::cout;

int const n = 5000;
int count = 0;
int timer, tin[n], fup[n];
bool used[n];
int mx[n][n];

void dfs(const int v, const int p = -1) {
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
  int procRank, procNum, sum = 0;
  double start, stop;
  timer = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0) {
    char ch;
    std::ifstream file("out.txt");
    if (!file.is_open()) {
      cout << "Can't open the file\n";
      exit(1);
    }

    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) {
        file.get(ch);
        if (ch == '1')
          mx[i][j] = 1;
        else
          mx[i][j] = 0;
    }
  }

  MPI_Bcast(mx, n*n, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < n; i++)
    used[i] = false;

  if (procRank == 0)
    start = MPI_Wtime();

  MPI_Barrier(MPI_COMM_WORLD);

  if (procNum == 1) {
    for (int i = 0; i < n; i++)
      if (!used[i])
        dfs(i);
  } else {
    if (procRank == 0)
      dfs(0);

    MPI_Bcast(&used, n, MPI_INT, 0, MPI_COMM_WORLD);

    if (procNum == 2) {
      if (procRank == 1)
        for (int i = 0; i < n; i++)
          if (!used[i])
            dfs(i);
    } else {
      if (procRank == 1)
        for (int i = 0; i < n; i++)
          if (!used[i]) {
            dfs(i);
            break;
          }
      
      MPI_Bcast(&used, n, MPI_INT, 1, MPI_COMM_WORLD);

      if (procRank == 2) {
        for (int i = 0; i < n; i++)
          if (!used[i]) {         
            dfs(i);
            break;
          }
      }
      
      MPI_Bcast(&used, n, MPI_INT, 2, MPI_COMM_WORLD);

      if (procNum == 4) {
        if (procRank == 3)
          for (int i = 0; i < n; i++)
            if (!used[i]) {
              dfs(i);
            }
      } else {
        if (procRank == 3)
          for (int i = 0; i < n; i++)
            if (!used[i]) {
              dfs(i);
              break;
            }

        MPI_Bcast(&used, n, MPI_INT, 3, MPI_COMM_WORLD);

        if (procRank == 4)
          for (int i = 0; i < n; i++)
            if (!used[i]) {
              dfs(i);
              break;
            }

        MPI_Bcast(&used, n, MPI_INT, 4, MPI_COMM_WORLD);

        if (procRank == 5)
          for (int i = 0; i < n; i++)
            if (!used[i]) {
              dfs(i);
              break;
            }

        MPI_Bcast(&used, n, MPI_INT, 5, MPI_COMM_WORLD);

        if (procRank == 6)
          for (int i = 0; i < n; i++)
            if (!used[i]) {
              dfs(i);
              break;
            }

        MPI_Bcast(&used, n, MPI_INT, 6, MPI_COMM_WORLD);

        if (procRank == 7)
          for (int i = 0; i < n; i++)
            if (!used[i]) {
              dfs(i);
            }
      }
    }
  }

  //cout << procRank << ": " << count << "\n";

  //MPI_Reduce(&count, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (procRank == 0) {
    stop = MPI_Wtime();
    cout << "time - " << stop - start << "\n";
  }

  MPI_Finalize();
  return 0;
}
