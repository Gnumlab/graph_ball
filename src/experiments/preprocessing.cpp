#include <fstream>
#include "../Graph_csr.cpp"
#include "../include/LazyBall.h"
#include "../Hash.cpp"

/**
 * Print the size of the balls of radius 2 for each vertex in the graph, sorted by size in non-increasing order.
 */
void printSortedBallSizes(std::string filename, bool isDirected)
{
    Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, 0, 0.0);
    G->fill_graph();

    std::vector<std::pair<int, int>> nodes = std::vector<std::pair<int, int>>(G->getN());
    for (int i = 0; i < G->getN(); i++)
        nodes[i] = {i, G->bfs_2(i)};

    std::sort(nodes.begin(), nodes.end(), [](auto &a, auto &b)
              { return a.second > b.second; });

    cout << G->getN() << endl;
    for (int i = 0; i < G->getN(); i++)
        cout << nodes[i].first << " " << nodes[i].second << endl;

    delete G;
    return;
}

/**
 * Giveng a graph G and a parameter k, this function returns the k vertices with the largest ball of radius 2.
 * @param G: Graph_csr object representing the graph.
 * @param k: number of vertices to return.
 * @return: vector of k vertices with the largest ball of radius 2.
 */
template <class T>
std::vector<int> *topKBall2(Graph_csr<T> *G, int k)
{
    // Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, 0, 0.0);
    G->fill_graph();

    std::vector<std::pair<int, int>> nodes = std::vector<std::pair<int, int>>(G->getN());
    for (int i = 0; i < G->getN(); i++)
        nodes[i] = {i, G->bfs_2(i)};

    std::sort(nodes.begin(), nodes.end(), [](auto &a, auto &b)
              { return a.second > b.second; });

    std::vector<int> *result = new std::vector<int>(k);
    for (int i = 0; i < k; i++)
        (*result)[i] = nodes[i].first;

    G->flush_graph();
    return result;
}

/**
 * This function computes the minhash signatures for all the vertices with the top k largest balls of radius 2.
 * More preciselly, the function computes `n_hashes` minhash signatures for the top `topK` vertices with the largest balls of radius 2,
 * considering a snapshot of the graph at different densities specified by the `density` vector.
 * The signatures are written to a file with the following format:
 * vertex_id minhash[0] minhash[1] ... minhash[n_hashes - 1]
 *
 * For each density `alpha` the output file is named as follows:
 * dataset/data/signatures/{datasetName}_{alpha*100}\%.sig{n_hashes}
 *
 * @param datasetName: name of the dataset.
 * @param isDirected: boolean indicating whether the graph is directed or not.
 * @param n_hashes: number of hash functions to use.
 * @param topK: number of vertices to consider.
 * @param density: vector of densities to consider.
 * @return: void
 */
void writeMinHashSignatures(std::string datasetName, bool isDirected, int n_hashes, int topK = 1000, std::vector<float> density = {1.0})
{
    srand(time(NULL));

    cerr << "\nComputing minhash signatures for '" << datasetName << "' dataset" << endl;

    std::string filename = "./dataset/data/" + datasetName + ".edges";
    std::string ballsFileName = "./dataset/data/ball-sizes/" + datasetName + ".balls";

    TabulationHash<uint32_t> **hash_functions = new TabulationHash<uint32_t> *[n_hashes];
    for (int i = 0; i < n_hashes; i++)
        hash_functions[i] = new TabulationHash<uint32_t>();

    cerr << "Reading graph from file " << filename << " ...";
    Graph_csr<MinHashBall> *G = Graph_csr<MinHashBall>::from_file(filename, isDirected, 0, 0.0, n_hashes, (Hash<uint32_t> **)hash_functions);
    cerr << " [DONE]" << endl;

    int n, m;
    int *edges = read_edges(filename, &n, &m);

    cerr << "Reading ball sizes from file " << ballsFileName << " ...";
    std::vector<std::pair<int, int>> nodes = read_ball_sizes(ballsFileName);
    cerr << " [DONE]" << endl;

    for (int i = 0; i < 2 * m; i += 2)
    {
        G->update(edges[i], edges[i + 1]);
        // G->insert_edge(edges[i], edges[i + 1]);

        for (float alpha : density)
        {
            if (static_cast<int>(m * 2 * alpha) - 2 <= i && i < static_cast<int>(m * 2 * alpha))
            {

                std::string outFileName = "./dataset/data/signatures/" + datasetName + "_" + std::to_string(static_cast<int>(alpha * 100)) + "\%.sig" + std::to_string(n_hashes);
                std::ofstream file(outFileName);
                cerr << "Writing signatures for " << alpha << " density" << endl;
                cerr << "Writing to " << outFileName << " ...";

                for (int u = 0; u < topK; u++)
                {
                    file << nodes[u].first << " ";
                    // uint32_t *signature = G->computeExactSignature(nodes[u].first, n_hashes, (Hash<uint32_t> **)hash_functions);
                    uint32_t *signature = G->balls[nodes[u].first].getSignature();
                    for (int j = 0; j < n_hashes; j++)
                        file << signature[j] << " ";
                    file << std::endl;
                }

                file.close();

                cerr << " [DONE]" << endl;
            }
        }
    }

    delete[] edges;
    delete[] hash_functions;
    delete G;

    return;
}