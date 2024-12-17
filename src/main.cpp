#include "include/Utils.h"
// #include "include/Graph_csr.h"
#include "Graph_csr.cpp"
#include <algorithm>
#include <cstdint>
#include <omp.h>
#include <iostream>
#include "include/ExactBall.h"
#include "include/LazyBall.h"

using namespace std;

int main(int argc, char const *argv[])
{

  Graph_csr<LazyBall>(10, 10);
  exit(0);
  Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file("soc-LiveJournal1.txt", true);
  G->update(1, 2);
  G->update(2, 3);

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
