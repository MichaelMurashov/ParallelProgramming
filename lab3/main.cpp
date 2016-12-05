#include <mpi.h>
#include <iostream>
#include <algorithm>

using std::cout;

const int num = 10; // chislo vershin v grafe
int timer, tin[num], fup[num];
bool used[num];
int mx[num][num] = {
  { 0,1,0,0 },
  { 1,0,0,0 },
  { 0,0,0,1 },
  { 0,0,1,0 }
};

// poisk v glubinu
void dfs(int& numOfBridges, const int step, int v, int p = -1) {
  used[v] = true;
  tin[v] = fup[v] = timer++;

  for (int i = 0; i < num; i += step) {
    if (mx[v][i] == 1) {
      int to = i;

      if (to == p)  
        continue;

      if (used[to]) 
        fup[v] = std::min(fup[v], tin[to]);
      else {
        dfs(numOfBridges, step, to, v);
        fup[v] = std::min(fup[v], fup[to]);
        if (fup[to] > tin[v])
          numOfBridges++;
      }
    }
  }
}

void find_bridges(int start, int stop, int& numOfBridges, const int step) {
  timer = 0;

  for (int i = start; i < stop; ++i)
    used[i] = false;

  for (int i = start; i < stop; ++i)
    if (!used[i]) 
      dfs(numOfBridges, step, i);
}

int main(int argc, char* argv[]) {
  int procRank, procNum, numOfBridges = 0, answer = 0;
  double start, stop;
  
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

  if (procRank == 0)
    start = MPI_Wtime();
  MPI_Barrier(MPI_COMM_WORLD);

  find_bridges(, numOfBridges, procNum);

  MPI_Gather(&numOfBridges, 1, MPI_INT, &answer, 1, MPI_INT, 0, MPI_COMM_WORLD);

  MPI_Barrier(MPI_COMM_WORLD);
  if (procRank == 0) {
    stop = MPI_Wtime();;
    cout << "time - " << stop - start << "\n";
  }

  MPI_Finalize();
  return 0;
}