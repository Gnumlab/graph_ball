#include "../include/Graph_csr.h"
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

// template <class T>
void explicitBallSize(std::string filename, bool isDirected, int k, float phi, int sample_size, float initial_density = 0.3, int query_freq = 1000)
{
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
        G->update(edges[i], edges[i + 1]);
        cerr << "\r" << i / 2;
    }

    cerr << "\nStarting the experiment" << endl;
    printf("timestamp,vertex,estimated_ball_size,exact_ball_size\n");
    for (; i < 2 * m; i += 2)
    {
        G->update(edges[i], edges[i + 1]);

        if (i % query_freq == 0)
        {
            // #pragma omp parallel for
            for (auto u : query_vertices)
            {
                int estimated_ball_size = G->balls[u].size();
                int exact_ball_size = G->bfs_2(u);

                if (estimated_ball_size > exact_ball_size)
                {
                    cerr << "ERROR: " << i << " " << u << " " << estimated_ball_size << " " << exact_ball_size << endl;
                    cerr << edges[i] << " " << edges[i + 1] << endl;
                    G->print_vertex(u, true);
                    exit(0);
                }
                printf("%d, %d, %d, %d\n", i / 2, u, estimated_ball_size, exact_ball_size);
            }
        }
        cerr << "\r" << i / 2;
    }
}