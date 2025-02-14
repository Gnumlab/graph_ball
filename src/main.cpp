// #include "Graph_csr.cpp"
#include <cstring>
#include <iostream>
#include "experiments/experiments.cpp"
#include "Hash.cpp"
#include "include/KMVCounter.h"
#include <typeinfo>

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

void computeExactBalls(std::string fname, bool isDirected)
{
  std::vector<float> densities = {0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
  cerr<< fname<<endl;
  writeExactBalls(fname, isDirected, 5000, densities);
}
void computeExactSizes(std::string fname, bool isDirected)
{

  cerr<< fname<<endl;
  printSortedBallSizes(fname, isDirected);
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
  srand(time(NULL));
  std::vector<int> ks = {0, 1, 2, 4, 8, 16};
  std::vector<float> phis = {0.1, 0.2, 0.4, 0.6, 0.8, 1.0};
  int sample_size = 5000;
  int n_queries = 1000;
  float initial_density = 0.2;

  printf("k,phi,timestamp,vertex,ball_size,cumulative_merges\n");
  explicitBallSize(fname, isDirected, {0}, {0.0}, sample_size, initial_density, n_queries, true);
  explicitBallSize(fname, isDirected, ks, phis, sample_size, initial_density, n_queries, false, n_run);
}

void minhashTimeExperiment(std::string fname, bool isDirected, int n_hashes = 100, int n_runs = 10)
{
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0};

  printf("n,m,k,phi,n_hashes,time\n");
  updatesTimeMinHashBall(fname, isDirected, ks, phis, n_hashes, n_runs);
  updatesTimeMinHashBall(fname, isDirected, {0}, {0.0}, n_hashes, n_runs);
}

void kmvCounterTimeExperiment(std::string fname, bool isDirected, uint16_t counter_size = 32, int n_run = 10)
{
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0};

  printf("n,m,k,phi,counter_size,time\n");
  updatesTimeKMVBall(fname, isDirected, ks, phis, counter_size, 10);
  updatesTimeKMVBall(fname, isDirected, {0}, {0.0}, counter_size, 1);
}

void kmvCounterQualityExperiment(std::string datasetName, bool isDirected, uint16_t counter_size = 32, int n_run = 10)
{
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0, 0.0};
  std::vector<float> timeStamps = {0.5, 0.6, 0.7, 0.8, 0.9};
  printf("alpha,k,phi,u,ball_size,effective_size\n");
  sizeEstimationExperiment(datasetName, isDirected, ks, phis, timeStamps, counter_size, n_run);
}

template <typename T>
void testKMV(int k, int runs = 10, bool use_old = 0)
{
  srand(time(NULL));
  uint32_t N = 1 << 24;

  int n_runs = runs;
  float size_esitmation = 0.0;
  float error = 0.0;

  printf("KMVCounter<%s>_%d\n", typeid(T).name() == typeid(uint32_t).name() ? "uint32_t" : "uint64_t", use_old);
  printf("\truns: %d\n\tk: %d\n", n_runs, k);

  for (int j = 0; j < n_runs; j++)
  {

    TabulationHash<T> *h = new TabulationHash<T>();
    KMVCounter<T> *kmv = new KMVCounter<T>(k, h);

    for (uint32_t i = 0; i < N; i++)
    {
       if (use_old)kmv->add_old(i);
       else kmv->add(i);
    }

    uint32_t size;
    if(use_old) size = kmv->size_old();
    else size = kmv->size();
    
    size_esitmation += size;
    error += abs((float)size - (float)N) / (float)(N);
  }

  float avg_size = (float)size_esitmation / (float)n_runs;
  float avg_error = (float)error / (float)n_runs;

  printf("**RESULTS**\n");
  printf("\tEffective-size: %d (%.4e)\n", N, (float)N);
  printf("\tAverage-size: %d (%.4e)\n", (uint32_t)avg_size, avg_size);
  printf("\tratio apx / effective: %.4f (1 \u00b1 %.4e)\n", avg_size / (float)N, std::abs(1.0 - avg_size / (float)N));
  printf("\tAverage-error: %.4f\t(1 / sqrt(k) = %.4f)\n", avg_error, 1.0 / sqrt(k));
  printf("============\n\n");
  return;
}

int main(int argc, char const *argv[])
{
  // testKMV<uint32_t>(8);
  // testKMV<uint32_t>(16);
  // testKMV<uint32_t>(32);
  // exit(0);

  std::string usage = "./build/apps/run [explicit|minhash-time|minhash-quality|counter-time|counter-quality|size-estim|exact-sizes] <dataset> <isDirected> <n_hashes|counter_size>";
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
  else if (experimentType == "counter-time")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);
    uint16_t counter_size = atoi(argv[4]);
    kmvCounterTimeExperiment(filename, isDirected, counter_size);
  }
  else if (experimentType == "size-estim")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);
    uint16_t counter_size = atoi(argv[4]);
    kmvCounterQualityExperiment(filename, isDirected, counter_size);
  }
  else if (experimentType == "exact-sizes")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);
    computeExactSizes(filename, isDirected);
  }
  else
  {
    cout << usage << endl;
  }

  return 0;
}
