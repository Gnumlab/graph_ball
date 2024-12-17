#include "include/Graph_csr.h"
#include "include/Utils.h"
#include <algorithm>
#include <cstdint>
#include <omp.h>
#include <iostream>

using namespace std;


int main(int argc, char const *argv[])
{
  Graph_csr G = Graph_csr::from_file("soc-LiveJournal1.txt", 0);

  return 0;
}
