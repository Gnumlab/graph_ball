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

using namespace std;

int main(int argc, char const *argv[])
{
  Graph_csr<MinHashBall> *G = Graph_csr<MinHashBall>::from_file("test-graph-1.graph", false);
  G->setThreshold(1.0);
  int n, m;
  int *edges = read_Graph("test-graph-1.graph", &n, &m, true);

  for (int i = 0; i < 2 * m - 12; i += 2)
  {
    G->update(edges[i], edges[i + 1]);
  }

  G->print_graph();
  exit(0);
  for (int i = 0; i < G->getN(); i++)
  {
    std::cout << i << " " << G->balls[i].size() << std::endl;
  }

  //   if(argc < 3){
  //     fprintf(stderr, "usage: <file_name> <direction>\n");
  //     exit(EXIT_FAILURE);
  //   }

  //   std::string filename = argv[1];
  //   int direction = atoi(argv[2]);
  //   if(direction > 2 || direction < 0){
  //     fprintf(stderr, "<direction>: must be 0 (undirected) or 1 (directed)\n");
  //     exit(EXIT_FAILURE);
  //   }
  //   Graph G;
  //   G.generate(filename, direction);
  // //  G.generate("g1");
  // //  G.generate("p2p-Gnutella08.txt");
  // //  G.generate("Slashdot0902.txt");
  // //  G.print_top_balls(5000);

  return 0;
}
