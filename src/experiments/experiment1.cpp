#include "../include/Graph_csr.h"
#include "../include/LazyBall.h"
#include "../include/Utils.h"
#include <algorithm>

std::vector<int> *topKBall2(std::string filename, int k, bool isDirected)
{
    Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, 0, 0.0);
    G->fill_graph();

    std::vector<std::pair<int, int>> nodes = std::vector<std::pair<int, int>>(G->getN());
    for (int i = 0; i < G->getN(); i++)
        nodes[i] = {i, G->bfs_2(i)};

    std::sort(nodes.begin(), nodes.end(), [](auto &a, auto &b)
              { return a.second > b.second; });

    std::vector<int> *result = new std::vector<int>(k);
    for (int i = 0; i < k; i++)
        (*result)[i] = nodes[i].first;

    delete G;
    return result;
}

// template <class T>
void explicitBallSize(std::string filename, bool isDirected, int k, float phi, vector<int> query_vertices, float initial_density = 0.3, int query_freq = 1000)
{
    Graph_csr<LazyBall> *G = Graph_csr<LazyBall>::from_file(filename, isDirected, k, phi);

    int n, m;
    int *edges = read_edges(filename, &n, &m);

    // initialize the graph
    int i = 0;
    for (; i < 2 * (m * initial_density); i += 2)
        G->update(edges[i], edges[i + 1]);

    for (; i < 2 * m; i += 2)
    {
        G->update(edges[i], edges[i + 1]);

        if (i % query_freq == 0)
        {
            for (auto u : query_vertices)
                printf("%d, %d, %d, %d\n", i / 2, u, G->balls[u].size(), G->bfs_2(u));
        }
    }

    G->print_graph();
}