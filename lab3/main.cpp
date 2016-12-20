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

void dfs(const int start, const int stop, const int v, const int p = -1) {
  used[v] = true;
  tin[v] = fup[v] = timer++;

  for (int i = start; i < stop; ++i) {
    if (mx[v][i] == 1) {
      int to = i;
      if (to == p)
        continue;

      if (used[to])
        fup[v] = std::min(fup[v], tin[to]);
      else {
        dfs(start, stop, to, v);
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

  int* tops = new int[procNum + 1];

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
    cout << "end read\n";

    int l = 0;
    for (int i = 0; i < procNum; i++)
      tops[i] = 0;

    if (procNum > 1) {
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          if (mx[i][j] == 1 && mx[j][i] == 1 && l < procNum) {
            tops[l++] = i;
            tops[l++] = j;
          }
    }

    tops[(procNum + 1) - 1] = n;
    for (int i = 0; i < procNum + 1; i++)
      cout << tops[i] << " ";
    cout << "\n";
  }

  
  MPI_Bcast(tops, procNum + 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Bcast(mx, n*n, MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = 0; i < n; i++)
    used[i] = false;

  if (procRank == 0)
    start = MPI_Wtime();

  MPI_Barrier(MPI_COMM_WORLD);

  for (int i = tops[procRank]; i < tops[procRank + 1]; i++)
    if (!used[i])
      dfs(tops[procRank], tops[procRank + 1], i);

  cout << procRank << ": " << count << "\n";

  MPI_Reduce(&count, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (procRank == 0) {
    stop = MPI_Wtime();
    cout << "time - " << stop - start << "\n";
  }

  MPI_Finalize();
  return 0;
}
