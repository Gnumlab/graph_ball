#include "../include/LazyBall.h"
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
void explicitBallSize(std::string filename, bool isDirected, std::vector<int> ks, std::vector<float> phis, int sample_size, float initial_density = 0.3, int n_queries = 1000, bool exactBall = false)
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
    std::vector<int> *query_vertices = topKBall2(G, sample_size);

    int n, m;
    int *edges = read_edges(filename, &n, &m);

    for (auto phi : phis)
    {
        for (auto k : ks)
        {
            G->setThreshold(phi);
            G->setK(k);

            cerr << "k: " << k << endl;
            cerr << "phi: " << phi << endl;

            for (int t = 0; t < (9 * (int)(k != 0)) + 1; t++)
            {
                // initialize the graph
                cerr << "Initializing the graph with " << initial_density << " density" << endl;
                int i = 0;
                for (; i < 2 * (m * initial_density); i += 2)
                {
                    if (!exactBall)
                        G->update(edges[i], edges[i + 1]);
                    else
                        G->insert_edge(edges[i], edges[i + 1]);
                }

                int query_freq = (2 * m - i) / n_queries;

                int n_merge = 0;
                cerr << "\nStarting the experiment" << endl;
                for (; i < 2 * m; i += 2)
                {
                    if (!exactBall)
                        n_merge += G->update(edges[i], edges[i + 1]);
                    else
                        G->insert_edge(edges[i], edges[i + 1]);

                    if (i % query_freq == 0)
                    {
                        for (auto u : *query_vertices)
                        {
                            int ball_size = !exactBall ? G->balls[u].size() : G->bfs_2(u);
                            printf("%d,%.2f,%d,%d,%d,%d\n", k, phi, i / 2, u, ball_size, n_merge);
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
void executeAllUpdates(Graph_csr<MinHashBall> *G, int m, int *edges)
{
    for (int i = 0; i < 2 * m; i += 2)
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
void updatesTime(std::string filename, bool isDirected, std::vector<int> ks, std::vector<float> phis, int n_hashes)
{

    TabulationHash<uint32_t> **hash_functions = new TabulationHash<uint32_t> *[n_hashes];
    for (int i = 0; i < n_hashes; i++)
        hash_functions[i] = new TabulationHash<uint32_t>();

    Graph_csr<MinHashBall> *G = Graph_csr<MinHashBall>::from_file(filename, isDirected, 0, 0.0, n_hashes, (Hash<uint32_t> **)hash_functions);

    int n, m;
    int *edges = read_edges(filename, &n, &m);

    for (float phi : phis)
    {
        G->setThreshold(phi);
        for (int k : ks)
        {
            G->setK(k);
            G->flush_graph();

            auto start = std::chrono::high_resolution_clock::now();
            executeAllUpdates(G, m, edges);

            // ending time
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - start);
            float t = duration.count() / 1000000.0;

            // n, m, k, phi, n_hashes, time
            printf("%d,%d,%d,%.3f,%d,%f\n", n, m, k, phi, n_hashes, t);
        }
    }

    delete[] edges;
    delete G;
    for (int i = 0; i < n_hashes; i++) {
        delete hash_functions[i]; // Delete each TabulationHash instance
    }
    delete[] hash_functions

}
