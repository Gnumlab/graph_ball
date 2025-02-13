#include "../include/Utils.h"
#include "../Graph_csr.cpp"
#include "../Hash.cpp"
#include "preprocessing.cpp"
#include <algorithm>
#include <chrono>

using namespace std;

/**
 * This function performs the experiment of updating a graph, and printing the size of the ball of radius 2 of a sample of vertices.
 * @param filename: name of the file containing the graph.
 * @param isDirected: boolean indicating if the graph is directed.
 * @param k: sampling parameter of the graph.
 * @param phi: threshold parameter of the graph.
 * @param sample_size: number of vertices to sample.
 * @param initial_density: initial density of the graph (default 0.3). This function initializes the graph with a fraction of the edges before starting the experiment.
 * @param query_freq: each `query_freq` updates, the size of the balls of radius 2 of the sampled vertices is printed.
 * @param exactBall: boolean indicating if the ball of radius 2 is computed using lazy updates or exact using the BFS algorithm.
 * @return: void
 */
void explicitBallSize(std::string filename, bool isDirected, std::vector<int> ks, std::vector<float> phis, uint32_t sample_size, float initial_density = 0.3, int n_queries = 1000, bool exactBall = false, int n_runs = 10)
{
    cerr << "Experiment Parameters" << endl;
    cerr << "Filename: " << filename << endl;
    cerr << "isDirected: " << isDirected << endl;
    cerr << "sample_size: " << sample_size << endl;
    cerr << "initial_density: " << initial_density << endl;
    cerr << "n_queries: " << n_queries << endl;
    cerr << "exactBall: " << exactBall << endl;
    cerr << "--------------------------------" << endl;

    cerr << "Reading graph from file: " << filename << endl;
    Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, 0, 0.0);
    std::vector<uint32_t> *query_vertices = topKBall2(G, sample_size);

    uint32_t n;
    uint64_t m;
    uint32_t *edges = read_edges(filename, &n, &m);

    for (auto phi : phis)
    {
        for (auto k : ks)
        {
            G->setThreshold(phi);
            G->setK(k);

            cerr << "k: " << k << endl;
            cerr << "phi: " << phi << endl;

            for (int t = 0; t < ((n_runs - 1) * (int)(k != 0)) + 1; t++)
            {
                // initialize the graph
                cerr << "Initializing the graph with " << initial_density << " density" << endl;
                uint64_t i = 0;
                for (; i < 2 * (m * initial_density); i += 2)
                {
                    if (!exactBall)
                        G->update(edges[i], edges[i + 1]);
                    else
                        G->insert_edge(edges[i], edges[i + 1]);
                }

                uint64_t query_freq = (2 * m - i) / (uint64_t)n_queries;

                uint64_t n_merge = 0;
                cerr << "\nStarting the experiment" << endl;
                for (; i < 2 * m; i += 2)
                {
                    if (!exactBall)
                        n_merge += G->update(edges[i], edges[i + 1]);
                    else
                        G->insert_edge(edges[i], edges[i + 1]);

                    if (i % query_freq == 0)
                    {
                        for (uint32_t u : *query_vertices)
                        {
                            uint32_t ball_size = !exactBall ? G->balls[u].size() : G->bfs_2(u);
                            printf("%d,%.2f,%lu,%u,%u,%lu\n", k, phi, i / 2, u, ball_size, n_merge);
                        }
                    }
                }
                cerr << "\n\n";
                G->flush_graph();
            }
        }
    }

    delete[] edges;
    delete G;
    delete[] query_vertices;
}

/**
 * This function performs all the updates in the graph.
 * @param G: Graph_csr object representing the graph.
 * @param m: number of insertions (edges) to perform.
 * @param edges: array of edges to insert.
 * @return: void
 */
template <class T>
void executeAllUpdates(Graph_csr<T> *G, uint64_t m, uint32_t *edges)
{
    for (uint64_t i = 0; i < 2 * m; i += 2)
        G->update(edges[i], edges[i + 1]);
}

/**
 * This function performs the experiment of updating a graph using minhash sketches, and printing the time taken to perform all the updates.
 * @param filename: name of the file containing the graph.
 * @param isDirected: boolean indicating if the graph is directed.
 * @param ks: vector of values of k to test.
 * @param phi: vector of values of phi to test.
 * @param n_hashes: number of hash functions to use in the minhash sketches.
 * @return: void
 */
void updatesTimeMinHashBall(std::string filename, bool isDirected, std::vector<int> ks, std::vector<float> phis, int n_hashes, int n_runs)
{

    std::cerr << filename << std::endl
              << std::flush;

    TabulationHash<uint32_t> **hash_functions = new TabulationHash<uint32_t> *[n_hashes];
    for (int i = 0; i < n_hashes; i++)
        hash_functions[i] = new TabulationHash<uint32_t>();

    Graph_csr<MinHashBall> *G = Graph_csr<MinHashBall>::from_file(filename, isDirected, 0, 0.0, n_hashes, (Hash<uint32_t> **)hash_functions);

    uint32_t n;
    uint64_t m;
    uint32_t *edges = read_edges(filename, &n, &m);

    for (int i = 0; i < n_runs; i++)
    {
        for (float phi : phis)
        {
            std::cerr << "phi: " << phi << std::endl
                      << std::flush;

            G->setThreshold(phi);
            for (int k : ks)
            {
                std::cerr << "\tk: " << k << std::endl
                          << std::flush;

                G->setK(k);
                for (int l = 0; l < n_hashes; l++)
                {
                    hash_functions[l]->reset();
                }
                G->flush_graph();

                auto start = std::chrono::high_resolution_clock::now();
                executeAllUpdates(G, m, edges);

                // ending time
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
                float t = duration.count() / 1000000.0;

                // n, m, k, phi, n_hashes, time
                printf("%u,%lu,%d,%.3f,%d,%f\n", n, m, k, phi, n_hashes, t);
                std::cout << std::flush;
            }
        }
    }

    delete[] edges;
    delete G;
    for (int i = 0; i < n_hashes; i++)
    {
        delete hash_functions[i]; // Delete each TabulationHash instance
    }
    delete[] hash_functions;
}

/**
 * This function performs the experiment of updating a graph using kmv sketches, and printing the time taken to perform all the updates.
 * @param filename: name of the file containing the graph.
 * @param isDirected: boolean indicating if the graph is directed.
 * @param ks: vector of values of k to test.
 * @param phi: vector of values of phi to test.
 * @param counter_size: size of the counter used in the kmv sketches.
 * @return: void
 */
void updatesTimeKMVBall(std::string filename, bool isDirected, std::vector<int> ks, std::vector<float> phis, uint16_t counter_size, int n_runs)
{
    std::cerr << filename << std::endl
              << std::flush;

    TabulationHash<uint32_t> *hash = new TabulationHash<uint32_t>();

    Graph_csr<KMVBall<uint32_t>> *G = Graph_csr<KMVBall<uint32_t>>::from_file(filename, isDirected, 0, 0.0, counter_size, hash);

    uint32_t n;
    uint64_t m;
    uint32_t *edges = read_edges(filename, &n, &m);

    for (int i = 0; i < n_runs; i++)
    {
        for (float phi : phis)
        {
            cerr << "phi: " << phi << endl;
            G->setThreshold(phi);
            for (int k : ks)
            {
                std::cerr << "\tk: " << k << std::endl
                          << std::flush;

                G->setK(k);
                hash->reset();
                G->flush_graph();

                auto start = std::chrono::high_resolution_clock::now();
                executeAllUpdates(G, m, edges);

                // ending time
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
                float t = duration.count() / 1000000.0;

                // n, m, k, phi, counter_size, time
                printf("%u,%lu,%d,%.3f,%d,%f\n", n, m, k, phi, counter_size, t);
                std::cout << std::flush;
            }
        }
    }

    delete hash;
    delete[] edges;
    delete G;
}

void sizeEstimationExperiment(string datasetName, bool isDirected, vector<int> ks, vector<float> phis, vector<float> timeStamps, uint16_t counter_size = 32, int n_runs = 10, int sample_size = 5000)
{

    string ballsFileName = "dataset/data/ball-sizes/" + datasetName + ".balls";
    auto balls = read_ball_sizes(ballsFileName);

    uint32_t sizes[timeStamps.size() + 1][sample_size];

    string fileName = "dataset/data/" + datasetName + ".edges";

    uint32_t n;
    uint64_t m;

    uint32_t *edges = read_edges(fileName, &n, &m);

    TabulationHash<uint32_t> *hash = new TabulationHash<uint32_t>();
    Graph_csr<KMVBall<uint32_t>> *G = Graph_csr<KMVBall<uint32_t>>::from_file(fileName, isDirected, 0, 0.0, counter_size, hash);

    std::cerr<<G->balls[1].size()<<std::endl;
    uint64_t i = 0;
    for (int j = 0; j < timeStamps.size(); j++)
    {
        float alpha = timeStamps[j];

        for (; i < 2 * m * alpha; i += 2)
        {
            G->insert_edge(edges[i], edges[i + 1]);
        }

        for (int l = 0; l < sample_size; l++)
        {
            uint32_t u = balls[l].first;
            uint32_t ball_size = G->bfs_2(u);
            sizes[j][l] = ball_size;
        }
    }

    for (int l = 0; l < sample_size; l++)
    {
        uint32_t ball_size = balls[l].second;
        sizes[timeStamps.size()][l] = ball_size;
    }

    timeStamps.push_back(1.0);

    /* RUN EXPERIMENTS */
    for (int t = 0; t < n_runs; t++)
    {
        for (float phi : phis)
        {
            cerr << "phi: " << phi << endl;
            G->setThreshold(phi);
            for (int k : ks)
            {
                std::cerr << "\tk: " << k << std::endl
                          << std::flush;

                G->setK(k);
                hash->reset();
                G->flush_graph();

                i = 0;
                for (int j = 0; j < timeStamps.size(); j++)
                {
                    float alpha = timeStamps[j];

                    for (; i < 2 * m * alpha; i += 2)
                    {
                        G->update(edges[i], edges[i + 1]);
                    }

                    for (int l = 0; l < sample_size; l++)
                    {
                        uint32_t u = balls[l].first;
                        uint32_t ball_size = G->balls[u].size();
                        uint32_t effective_size = sizes[j][l];
                        printf("%.2f,%d,%.2f,%u,%u,%u\n", alpha, k, phi, u, ball_size, effective_size);
                    }
                }

                if (phi == 0.0)
                {
                    break;
                }
            }
        }
    }

    delete hash;
    delete[] edges;
    delete G;
}
