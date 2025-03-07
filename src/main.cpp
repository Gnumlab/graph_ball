#include <cstring>
#include <iostream>
#include "experiments/experiments.cpp"
#include "Hash.cpp"
#include "include/KMVCounter.h"
#include <typeinfo>

using namespace std;

/**
 * This function computes the exact balls for the top K vertices in the graph,
 * and writes the results to a file.
 *
 * The output file is named as `dataset/data/balls/{datasetName}_{alpha*100}%.balls`
 * where `alpha` is the density of the graph in the range [0.5, 0.75, 1.0]
 *
 * The balls are written in the format `vertex_id ball_size vertex_1 vertex_2 ... vertex_{ball_size}`
 *
 * @param fname: the name of the dataset
 * @param isDirected: whether the graph is directed or not
 * @param topK: the number of top vertices to consider
 */
void computeExactBalls(std::string fname, bool isDirected, int topK = 5000, vector<float> densities = {0.5, 0.75, 1.0})
{
  cerr << fname << endl
       << flush;
  writeExactBalls(fname, isDirected, topK, densities);
}

/**
 * This function computes the exact sizes of the balls of radius 2 for each vertex in the graph,
 * and writes the results to a file in non-decreasing order.
 *
 * The output file is named as `dataset/data/ball-sizes/{datasetName}.balls`
 * where `datasetName` is the name of the dataset.
 *
 * The balls are written in the format `vertex_id ball_size`
 *
 * @param fname: the name of the dataset
 * @param isDirected: whether the graph is directed or not
 * @return: void
 */
void computeExactSizes(std::string fname, bool isDirected)
{
  cerr << "Computing exact ball sizes for " << fname << " ..." << endl;
  writeSortedBallSizes(fname, isDirected);
  cerr << "Done" << endl;
}

/**
 * This function computes the pairs of vertices with Jaccard similarity greater than or equal to a given threshold.
 * The vertices are the top k vertices with the largest balls of radius 2.
 * Each pair is sampled with probability p.
 *
 * The pairs are written to a file with the following format:
 * vertex_id_1 vertex_id_2 Jaccard_similarity
 *
 * The output file is named as follows:
 * dataset/data/pairs/{datasetName}_{alpha*100}%.pairs
 * where `alpha` is the density of the graph in the range [0.5, 0.75, 1.0]
 *
 * @param datasetName: name of the dataset.
 * @param threshold: Jaccard similarity threshold.
 * @param p: probability of sampling a pair.
 * @param densities: vector of densities to consider.
 * @return: void
 */
void preprocessPairs(string datasetName, float threshold = 0.2, float p = 0.01, vector<float> densities = {0.5, 0.75, 1.0})
{
  for (float d : densities)
  {
    string density = "_" + to_string(static_cast<int>(d * 100)) + "\%";
    computePairs2(datasetName + density, threshold, p);
  }
}

void explicitBallSizeExperiment(std::string fname, bool isDirected, int n_run = 10)
{
  srand(time(NULL));
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0};
  int sample_size = 5000;
  int n_queries = 100;
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

void minhashQualityExperiment(std::string fname, bool isDirected, int n_hashes = 100, int n_runs = 10)
{
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0, 0.0};
  std::vector<float> timeStamps = {0.5, 0.75, 1.0};

  printf("alpha,k,phi,u,v,true_similarity,estimated_similarity\n");
  similarityEstimationExperiment(fname, isDirected, ks, phis, timeStamps, n_hashes, n_runs, 1000);
}

void kmvCounterTimeExperiment(std::string fname, bool isDirected, uint16_t counter_size = 32, int n_run = 10)
{
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0};

  printf("n,m,k,phi,counter_size,time\n");
  updatesTimeKMVBall(fname, isDirected, ks, phis, counter_size, n_run);
  updatesTimeKMVBall(fname, isDirected, {0}, {0.0}, counter_size, 1);
}

void kmvCounterQualityExperiment(std::string datasetName, bool isDirected, uint16_t counter_size = 32, int n_run = 10)
{
  std::vector<int> ks = {0, 2, 4, 8};
  std::vector<float> phis = {0.1, 0.25, 0.5, 0.75, 1.0, 0.0};
  std::vector<float> timeStamps = {0.5, 0.75, 1.0};
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
      kmv->add(i);
    }

    uint32_t size;
    size = kmv->size();

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
  std::string usage = "./build/apps/run [explicit|minhash-time|similarity-estim|counter-time|size-estim|compute-exact-sizes|compute-pairs] <dataset> <isDirected> <n_hashes|counter_size>";
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
  else if (experimentType == "similarity-estim")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);
    int n_hashes = atoi(argv[4]);
    minhashQualityExperiment(filename, isDirected, n_hashes);
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
  else if (experimentType == "compute-exact-sizes")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);
    computeExactSizes(filename, isDirected);
  }
  else if (experimentType == "compute-pairs")
  {
    std::string filename = argv[2];
    bool isDirected = (bool)atoi(argv[3]);

    computeExactBalls(filename, isDirected);
    preprocessPairs(filename);
  }
  else
  {
    cout << usage << endl;
  }

  return 0;
}
