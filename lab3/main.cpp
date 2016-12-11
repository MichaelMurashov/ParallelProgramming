#include <mpi.h>
#include <iostream>
#include <algorithm>

using std::cout;

int const n = 4;
int num_of_bridges = 0;
int timer, tin[n], fup[n];
bool used[n];
int mx[n][n] = {
  { 0,1,0,0 },
  { 1,0,0,0 },
  { 0,0,0,1 },
  { 0,0,1,0 }
};

void dfs(int v, int p = -1) {
  used[v] = true;
  tin[v] = fup[v] = timer++;

  for (int i = 0; i < n; ++i) {
    if (mx[v][i] == 1) {
      int to = i;
      if (to == p)  
        continue;

      if (used[to]) fup[v] = std::min(fup[v], tin[to]);
      else {
        dfs(to, v);
        fup[v] = std::min(fup[v], fup[to]);

        if (fup[to] > tin[v])
          num_of_bridges++;
      }
    }
  }
}

void find_bridges() {
  timer = 0;
  for (int i = 0; i < n; ++i)
    used[i] = false;

  for (int i = 0; i < n; ++i)
    if (!used[i]) dfs(i);
}

int main(int argc, char* argv[]) {
  find_bridges();
  cout << num_of_bridges << "\n";
  return 0;
}