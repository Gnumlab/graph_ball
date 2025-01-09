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
  if (argc < 3) {
    cout << "Usage: ./main <filename> <sample_size>" << endl;
    return 1;
  }
  

  std::string filename = argv[1];
  int sample_size = atoi(argv[2]);
  bool isDirected = true;
  explicitBallSize(filename, isDirected, 0, 1.0, sample_size, 0.00, 1000, false);
  return 0;
}
