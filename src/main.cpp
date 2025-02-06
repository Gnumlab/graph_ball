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
  int n_hash = 2000;

  for (auto d : dataset)
    writeMinHashSignatures(d.first, d.second, n_hash, 5000, densities);

  return;
}

void computeExactBalls()
{
  std::vector<float> densities = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
  std::vector<std::pair<std::string, bool>> dataset = {
      {"comm-linux-kernel-reply", true},
      {"fb-wosn-friends", false},
      {"ia-enron-email-all", true},
      {"soc-youtube-growth", true},
      {"soc-flickr-growth", true},
  };

  for (auto d : dataset)
    writeExactBalls(d.first, d.second, 5000, densities);

  return;
}

void preprocessPairs(int b, int r, float J)
{
  std::vector<std::pair<std::string, bool>> dataset = {
      {"comm-linux-kernel-reply", true},
      {"fb-wosn-friends", false},
      {"ia-enron-email-all", true},
      {"soc-youtube-growth", true},
      {"soc-flickr-growth", true},
  };

  for (auto d : dataset)
    computePairs(d.first + "_100\%", 2000, b, r, J);

  return;
}

void preprocessPairs2(float threshold = 0.2, float p = 0.01)
{
  std::vector<std::pair<std::string, bool>> dataset = {
      {"comm-linux-kernel-reply", true},
      {"fb-wosn-friends", false},
      {"ia-enron-email-all", true},
      {"soc-youtube-growth", true},
      {"soc-flickr-growth", true},
  };

  for (auto d : dataset)
  {
    for (auto density : {"_50\%", "_60\%", "_70\%", "_80\%", "_90\%", "_100\%"})
      computePairs2(d.first + density, threshold, p);
  }

  return;
}

void explicitBallSizeExperiment(std::string fname, bool isDirected, int n_run = 10)
{
  std::vector<int> ks = {0, 1, 2, 4, 8, 16};
  std::vector<float> phis = {0.1, 0.2, 0.4, 0.6, 0.8, 1.0};
  int sample_size = 5000;
  int n_queries = 1000;
  float initial_density = 0.2;

  printf("k,phi,timestamp,vertex,ball_size,cumulative_merges\n");
  explicitBallSize(fname, isDirected, {0}, {0.0}, sample_size, initial_density, n_queries, true);
  explicitBallSize(fname, isDirected, ks, phis, sample_size, initial_density, n_queries, false);
}

void minhashTimeExperiment(std::string fname, bool isDirected, int n_hashes = 100, int n_run = 10)
{
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0};

  printf("dataset,n,m,k,phi,n_hashes,time\n");
  for (int i = 0; i < n_run; i++)
    updatesTime(fname, isDirected, {0}, {0.0}, n_hashes);

  for (int i = 0; i < n_run; i++)
    updatesTime(fname, isDirected, ks, phis, n_hashes);
}

int main(int argc, char const *argv[])
{
  std::string usage = "./build/apps/run [explicit|minhash-time|minhash-quality|counter-time|counter-quality] <dataset> <isDirected> <n_hashes>";
  if (argc < 2)
  {
    cout << usage << endl;
    return 0;
  }

  std::string experimentType = argv[1];

  if (experimentType == "explicit")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);
    explicitBallSizeExperiment(filename, isDirected);
  }
  else if (experimentType == "minhash-time")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);
    int n_hashes = atoi(argv[4]);
    minhashTimeExperiment(filename, isDirected, n_hashes);
  }
  else
  {
    cout << usage << endl;
  }

  return 0;

  // computeMinHashSignatures();
  // computeExactBalls();

  // for j = 0.198 e p = 0.8, or j = 0.236 e p = 0.9
  // int b = 40;
  // int r = 2;
  // preprocessPairs(b, r, 0.2);

  // preprocessPairs2();
  // return 0;

  // if (argc < 3)
  // {
  //   cerr << "Usage: ./build/apps/Graph <filename> <sample_size> [-exact]" << endl;
  //   return 1;
  // }

  // std::string filename = argv[1];
  // bool isDirected = true;
  // int k = 0;
  // float phi = 1.0;
  // int sample_size = atoi(argv[2]);
  // float initial_density = 0.05;
  // int query_freq = 1000;
  // bool exactBall = (argc == 4 && strcmp(argv[3], "-exact") == 0);

  // explicitBallSize(filename, isDirected, k, phi, sample_size, initial_density, query_freq, exactBall);
  // return 0;
}
