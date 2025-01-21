// #include "Graph_csr.cpp"
#include <cstring>
#include <iostream>
#include "experiments/experiments.cpp"

using namespace std;

void computeMinHashSignatures()
{
  std::vector<float> densities = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
  std::vector<std::pair<std::string, bool>> dataset = {
      {"comm-linux-kernel-reply", true},
      {"fb-wosn-friends", false},
      {"ia-enron-email-all", true},
      {"soc-youtube-growth", true},
      {"soc-flickr-growth", true},
  };
  std::vector<int> n_hashes = {2000};

  for (auto d : dataset)
  {
    for (int n_hash : n_hashes) {
      writeMinHashSignatures(d.first, d.second, n_hash, 5000, densities);
    }
  }
  return;
}

int main(int argc, char const *argv[])
{
  computeMinHashSignatures();
  return 0;

  bool directed = (bool)atoi(argv[2]);
  std::vector<int> ks = {3, 10};
  std::vector<float> phis = {0.6, 0.8, 1.0};
  int n_hashes = 100;

  exit(0);

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

  explicitBallSize(filename, isDirected, k, phi, sample_size, initial_density, query_freq, exactBall);
  return 0;
}
