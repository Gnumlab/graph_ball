#include "Graph_csr.cpp"
#include <cstring>
#include <iostream>
#include "experiments/experiment1.cpp"

using namespace std;

int main(int argc, char const *argv[])
{
  if (argc < 3)
  {
    cerr << "Usage: ./build/apps/Graph <filename> <sample_size> [-exact]" << endl;
    return 1;
  }

  std::string filename = argv[1];
  bool isDirected = true;
  int k = 0;
  float phi = 1.0;
  int sample_size = atoi(argv[2]);
  float initial_density = 0.05;
  int query_freq = 1000;
  bool exactBall = (argc == 4 && strcmp(argv[3], "-exact") == 0);

  // explicitBallSize(filename, isDirected, k, phi, sample_size, initial_density, query_freq, exactBall);

  int n_hashes = 1000;
  minhashSketchBallSize(filename, isDirected, n_hashes, k, phi, sample_size, initial_density, query_freq);
  return 0;
}
