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
  
  

  Graph G;
  G.generate("soc-LiveJournal1.txt", "");



  return 0;
}
