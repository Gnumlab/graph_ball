#include "include/Graph.h"
#include "include/Utils.h"
#include <algorithm>
#include <cstdint>
#include <omp.h>
#include <iostream>

using namespace std;


int main(int argc, char const *argv[])
{
  // experiment1();
  // experiment2();
  
  if(argc < 3){
    fprintf(stderr, "usage: <file_name> <direction>\n");  
    exit(EXIT_FAILURE);
  }
  
  std::string filename = argv[1];
  int direction = atoi(argv[2]);
  if(direction > 2 || direction < 0){
    fprintf(stderr, "<direction>: must be 0 (undirected) or 1 (directed)\n");  
    exit(EXIT_FAILURE);
  }
  Graph G;
  G.generate(filename, direction);
//  G.generate("g1");
//  G.generate("p2p-Gnutella08.txt");
//  G.generate("Slashdot0902.txt");
//  G.print_top_balls(5000);
  G.compute_gamma(1, G.getN());


  return 0;
}
