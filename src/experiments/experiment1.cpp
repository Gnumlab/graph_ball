#include "../include/LazyBall.h"
#include "../include/Utils.h"
#include <algorithm>

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

void explicitBallSize(std::string filename, bool isDirected, int k, float phi, int sample_size, float initial_density = 0.3, int query_freq = 1000, bool exactBall = false)
{
    cerr << "Experiment Parameters" << endl;
    cerr << "Filename: " << filename << endl;
    cerr << "isDirected: " << isDirected << endl;
    cerr << "k: " << k << endl;
    cerr << "phi: " << phi << endl;
    cerr << "sample_size: " << sample_size << endl;
    cerr << "initial_density: " << initial_density << endl;
    cerr << "query_freq: " << query_freq << endl;
    cerr << "exactBall: " << exactBall << endl;
    cerr << "--------------------------------" << endl;

    cerr << "Reading graph from file: " << filename << endl;
    Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, k, phi);
    std::vector<int> query_vertices = *topKBall2(G, sample_size);

    int n, m;
    int *edges = read_edges(filename, &n, &m);

    // initialize the graph
    cerr << "Initializing the graph with " << initial_density << " density" << endl;
    int i = 0;
    for (; i < 2 * (m * initial_density); i += 2)
    {
        if (!exactBall)
            G->update(edges[i], edges[i + 1]);
        else
            G->insert_edge(edges[i], edges[i + 1]);

        cerr << "\r" << i / 2;
    }

    int n_merge = 0;
    cerr << "\nStarting the experiment" << endl;
    printf("timestamp,vertex,ball_size,cumulative_merges\n");
    for (; i < 2 * m; i += 2)
    {
        if (!exactBall)
            n_merge += G->update(edges[i], edges[i + 1]);
        else
            G->insert_edge(edges[i], edges[i + 1]);

        if (i % query_freq == 0)
        {
            // #pragma omp parallel for
            for (auto u : query_vertices)
            {
                int ball_size = !exactBall ? G->balls[u].size() : G->bfs_2(u);
                printf("%d, %d, %d, %d\n", i / 2, u, ball_size, n_merge);
            }
        }
        cerr << "\r" << i / 2;
    }
}