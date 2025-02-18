#include <fstream>
#include "../Graph_csr.cpp"
#include "../include/LazyBall.h"
#include "../Hash.cpp"
#include "LSH.cpp"

/**
 * This funcion wirte to file the size of the balls of radius 2 for each vertex in the graph, sorted by size in non-increasing order.
 * The output file is named as `dataset/data/ball-sizes/{datasetName}.balls`
 * The format of the output file is `vertex_id ball_size`.
 *
 * @param datasetName: name of the dataset.
 * @param isDirected: boolean indicating whether the graph is directed or not.
 * @return: void
 */
void writeSortedBallSizes(std::string datasetName, bool isDirected)
{
    std::string filename = "./dataset/data/" + datasetName + ".edges";
    std::string outputFileName = "./dataset/data/ball-sizes/" + datasetName + ".balls";

    Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, 0, 0.0);
    G->fill_graph();

    std::vector<std::pair<uint32_t, uint32_t>> nodes = std::vector<std::pair<uint32_t, uint32_t>>(G->getN());
    for (uint32_t i = 0; i < G->getN(); i++)
        nodes[i] = {i, G->bfs_2(i)};

    std::sort(nodes.begin(), nodes.end(), [](auto &a, auto &b)
              { return a.second > b.second; });

    std::ofstream file(outputFileName);

    for (uint32_t i = 0; i < G->getN(); i++)
        file << nodes[i].first << " " << nodes[i].second << std::endl;
    // cout << nodes[i].first << " " << nodes[i].second << endl;

    cerr << "File " << outputFileName << " created" << endl;

    delete G;
    file.close();
    return;
}

/**
 * Giveng a graph G and a parameter k, this function returns the k vertices with the largest ball of radius 2.
 * @param G: Graph_csr object representing the graph.
 * @param k: number of vertices to return.
 * @return: vector of k vertices with the largest ball of radius 2.
 */
template <class T>
std::vector<uint32_t> *topKBall2(Graph_csr<T> *G, uint32_t k)
{
    // Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, 0, 0.0);
    G->fill_graph();

    std::vector<std::pair<uint32_t, uint32_t>> nodes = std::vector<std::pair<uint32_t, uint32_t>>(G->getN());
    for (uint32_t i = 0; i < G->getN(); i++)
        nodes[i] = {i, G->bfs_2(i)};

    std::sort(nodes.begin(), nodes.end(), [](auto &a, auto &b)
              { return a.second > b.second; });

    std::vector<uint32_t> *result = new std::vector<uint32_t>(k);
    for (uint32_t i = 0; i < k; i++)
        (*result)[i] = nodes[i].first;

    G->flush_graph();
    return result;
}

/**
 * TODO: No needed anymore: remove this function
 *
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
void writeMinHashSignatures(std::string datasetName, bool isDirected, int n_hashes, uint32_t topK = 1000, std::vector<float> density = {1.0})
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
    // Graph_csr<MinHashBall> *G = Graph_csr<MinHashBall>::from_file(filename, isDirected, 0, 0.0, 0, (Hash<uint32_t> **)hash_functions);
    cerr << " [DONE]" << endl;

    uint32_t n;
    uint64_t m;
    uint32_t *edges = read_edges(filename, &n, &m);

    cerr << "Reading ball sizes from file " << ballsFileName << " ...";
    std::vector<std::pair<uint32_t, uint32_t>> nodes = read_ball_sizes(ballsFileName);
    cerr << " [DONE]" << endl;

    for (uint64_t i = 0; i < 2 * m; i += 2)
    {
        // G->update(edges[i], edges[i + 1]);
        G->insert_edge(edges[i], edges[i + 1]);

        for (float alpha : density)
        {
            if (static_cast<uint64_t>(m * 2 * alpha) - 2 <= i && i < static_cast<uint64_t>(m * 2 * alpha))
            {
                std::string outFileName = "./dataset/data/signatures/" + datasetName + "_" + std::to_string(static_cast<int>(alpha * 100)) + "\%.sig" + std::to_string(n_hashes);
                std::ofstream file(outFileName);
                cerr << "Writing signatures for " << alpha << " density" << endl;
                cerr << "Writing to " << outFileName << " ...";

                for (uint32_t u = 0; u < topK; u++)
                {
                    file << nodes[u].first << " ";
                    // uint32_t *signature = G->computeExactOPHSignature(nodes[u].first, n_hashes, (Hash<uint32_t> **)hash_functions, 2000);
                    uint32_t *signature = G->balls[nodes[u].first].getSignature();
                    for (int j = 0; j < n_hashes; j++)
                        file << signature[j] << " ";
                    file << std::endl;
                    // delete[] signature;
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

/**
 * This function computes the exact balls of radius 2 for all the vertices with the top k largest balls of radius 2,
 * considering a snapshot of the graph at different densities specified by the `density` vector.
 * Then, the balls are written to a file with the following format:
 * vertex_id ball_size vertex_1 vertex_2 ... vertex_{ball_size}
 *
 * For each density `alpha` the output file is named as follows:
 * dataset/data/balls/{datasetName}_{alpha*100}\%.balls
 *
 * @param datasetName: name of the dataset.
 * @param isDirected: boolean indicating whether the graph is directed or not.
 * @param topK: number of vertices to consider.
 * @param density: vector of densities to consider.
 */
void writeExactBalls(std::string datasetName, bool isDirected, uint32_t topK = 1000, std::vector<float> density = {1.0})
{
    srand(time(NULL));

    cerr << "\nComputing Exact Balls of radius 2 for '" << datasetName << "' dataset" << endl;

    std::string filename = "./dataset/data/" + datasetName + ".edges";
    std::string ballsFileName = "./dataset/data/ball-sizes/" + datasetName + ".balls";

    cerr << "Reading graph from file " << filename << " ...";
    Graph_csr<MinHashBall> *G = Graph_csr<MinHashBall>::from_file(filename, isDirected, 0, 0.0, 0, nullptr);
    cerr << " [DONE]" << endl;

    uint32_t n;
    uint64_t m;
    uint32_t *edges = read_edges(filename, &n, &m);

    cerr << "Reading ball sizes from file " << ballsFileName << " ...";
    std::vector<std::pair<uint32_t, uint32_t>> nodes = read_ball_sizes(ballsFileName);
    cerr << " [DONE]" << endl;

    for (uint64_t i = 0; i < 2 * m; i += 2)
    {
        G->insert_edge(edges[i], edges[i + 1]);

        for (float alpha : density)
        {
            if (static_cast<uint64_t>(m * 2 * alpha) - 2 <= i && i < static_cast<uint64_t>(m * 2 * alpha))
            {
                std::string outFileName = "./dataset/data/balls/" + datasetName + "_" + std::to_string(static_cast<int>(alpha * 100)) + "\%.balls";
                std::ofstream file(outFileName);

                cerr << "Writing balls for " << alpha << " density" << endl;
                cerr << "Writing to " << outFileName << " ...";

                for (uint32_t u = 0; u < topK; u++)
                {
                    file << nodes[u].first << " ";
                    std::vector<uint32_t> ball = G->ball_2(nodes[u].first);
                    file << ball.size() << " ";
                    for (uint32_t j = 0; j < ball.size(); j++)
                        file << ball[j] << " ";
                    file << std::endl;
                }

                file.close();
                cerr << " [DONE]" << endl;
            }
        }
    }

    delete[] edges;
    delete G;

    cerr << "\n\n";

    return;
}

/**
 * Given two sets a and b, this function computes the size of the intersection of the two sets.
 * The complexity of this function is O(min(a.size(), b.size())).
 * @param a: first set.
 * @param b: second set.
 * @return: size of the intersection of the two sets.
 */
uint32_t size_intersection(std::unordered_set<uint32_t> *a, std::unordered_set<uint32_t> *b)
{
    int size = 0;

    if (a->size() < b->size())
    {
        for (uint32_t x : *a)
            if (b->find(x) != b->end())
                size++;
    }
    else
    {
        for (uint32_t x : *b)
            if (a->find(x) != a->end())
                size++;
    }
    return size;
}

/**
 * Given two sets a and b, this function computes the Jaccard similarity between the two sets.
 * @param a: first set.
 * @param b: second set.
 * @return: Jaccard similarity between the two sets.
 */
float jaccard_similarity(std::unordered_set<uint32_t> *a, std::unordered_set<uint32_t> *b)
{
    int intersection = size_intersection(a, b);
    return (float)intersection / (a->size() + b->size() - intersection);
}

/// @todo: No needed anymore: remove this function
void computePairs(std::string datasetName, int sig_size, int b, int r, float J)
{
    std::string filename = "./dataset/data/signatures/" + datasetName + ".sig" + std::to_string(sig_size);

    cerr << "Reading signatures from " << filename << " ...";
    std::vector<std::pair<uint32_t, uint32_t *>> signatures = read_signatures(filename, sig_size);
    int n = signatures.size();

    uint32_t **signaturesArray = new uint32_t *[signatures.size()];
    for (int i = 0; i < n; i++)
        signaturesArray[i] = signatures[i].second;
    cerr << " [DONE]\n"
         << endl;

    cerr << "Computing LSH for " << n << " signatures ..." << endl;
    std::unordered_set<std::pair<uint32_t, uint32_t>, hash_pair> *pairs = computeLSH(signaturesArray, n, r, b);
    cerr << " [DONE]" << endl;
    cerr << "Computed " << pairs->size() << " pairs\n"
         << endl;

    std::string ballsFileName = "./dataset/data/balls/" + datasetName + ".balls";
    cerr << "Reading balls from " << ballsFileName << " ...";
    auto balls = read_balls(ballsFileName);
    cerr << " [DONE]\n"
         << endl;

    std::string outFileName = "./dataset/data/pairs/" + datasetName + "_" + std::to_string(J) + ".pairs";
    std::ofstream file(outFileName);

    cerr << "Writing pairs to " << outFileName << " ...";
    uint64_t c = 0;
    for (auto pair : *pairs)
    {
        uint32_t b1 = signatures[pair.first].first;
        uint32_t b2 = signatures[pair.second].first;
        float j_sim = jaccard_similarity(&balls[b1], &balls[b2]);
        if (j_sim >= J)
        {
            c++;
            file << b1 << " " << b2 << " " << j_sim << std::endl;
        }
    }
    cerr << " [DONE]" << endl;
    cerr << "Written " << c << " pairs with Jaccard similarity >= " << J << endl;

    file.close();
    delete pairs;
    delete[] signaturesArray;

    cerr << "\n ==================== \n\n";
    return;
}

/**
 * This function samples pairs of vertices with Jaccard similarity greater than or equal to a given threshold.
 * The vertices are the top k vertices with the largest balls of radius 2.
 * Each pair is sampled with probability p.
 *
 * The pairs are written to a file with the following format:
 * vertex_id_1 vertex_id_2 Jaccard_similarity
 *
 * The output file is named as follows:
 * dataset/data/pairs/{datasetName}.pairs
 *
 * @param datasetName: name of the dataset.
 * @param threshold: Jaccard similarity threshold.
 * @param p: probability of sampling a pair.
 * @return: void
 */
void computePairs2(std::string datasetName, float threshold = 0.2, float p = 0.01)
{
    srand(time(NULL));

    // read balls
    std::string ballsFileName = "./dataset/data/balls/" + datasetName + ".balls";
    cerr << "Reading balls from " << ballsFileName << " ...";
    auto balls = read_balls(ballsFileName);
    cerr << " [DONE]\n"
         << endl;

    // compute js for each pair of balls
    std::string outFileName = "./dataset/data/pairs/" + datasetName + ".pairs";
    std::ofstream file(outFileName);

    cerr << "Writing pairs to " << outFileName << " ..." << endl;
    uint64_t c = 0;
    uint32_t i = 0;

    for (auto itr = balls.begin(); itr != balls.end(); itr++)
    {
        for (auto itr2 = itr; itr2 != balls.end(); itr2++)
        {
            if ((float)(rand() % 100) < (p * 100.0))
            {
                if (i++ % 10000 == 0)
                    cerr << "\r" << i << flush;

                if (itr != itr2)
                {
                    float j_sim = jaccard_similarity(&itr->second, &itr2->second);
                    if (j_sim >= threshold)
                    {
                        c++;
                        file << itr->first << " " << itr2->first << " " << j_sim << std::endl;
                    }
                }
            }
        }
    }
    cerr << " [DONE]" << endl;
    cerr << "Written " << c << " pairs with Jaccard similarity >= " << threshold << endl;

    file.close();
}
