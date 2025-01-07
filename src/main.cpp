#include "include/Utils.h"
// #include "include/Graph_csr.h"
#include "Graph_csr.cpp"
#include <algorithm>
#include <cstdint>
#include <omp.h>
#include <iostream>
#include "include/ExactBall.h"
#include "include/LazyBall.h"
#include "include/MinHashBall.h"
#include "include/Utils.h"
#include "experiments/experiment1.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
  std::string filename = "ca-cit-HepTh-test";
  int k = 100;

  auto vec = topKBall2(filename, k, true);
  explicitBallSize(filename, true, 0, 1.0, (*vec), 0.3, 1000);

  // int x = argc > 1 ? atoi(argv[1]) : 0;

  // Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file("test-graph-1.graph", false);
  // G->setThreshold(12);

  // //
  // int n_hashes = 2000;
  // PairWiseHash<int> **hash_functions = new PairWiseHash<int> *[n_hashes];

  // for (int i = 0; i < n_hashes; i++)
  //   hash_functions[i] = new PairWiseHash<int>();

  // Graph_csr<MinHashBall> *G = Graph_csr<MinHashBall>::from_file("test-graph-1.graph", true, 0, 1.0, n_hashes, (Hash<int> **)hash_functions);

  // int n, m;
  // int *edges = read_edges("test-graph-1.graph", &n, &m);
  // for (int i = 0; i < 2 * m - x; i += 2)
  // {
  //   G->update(edges[i], edges[i + 1]);
  // }

  // G->print_graph(false);

  return 0;
}
