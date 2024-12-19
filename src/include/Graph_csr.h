//
// Created by balmung on 08/03/19.
//

#ifndef DECREMENTALSCC_GRAPH_CSR_H
#define DECREMENTALSCC_GRAPH_CSR_H

#include <string>
#include "../Hash.cpp"
#include "MinHashBall.h"

template <class T>
class Graph_csr
{

private:
    int n, m; // number of vertices and number of edges
    bool directed;

    int *o_First;      // index, in o_Target, where the outedges of vertex v starts. Size n
    int *o_Target;     // second enpoint for each edge. Size m
    int *o_degree;     // current out-degree for each vertex v. Size n
    int *o_red_degree; // current red out-degree for each vertex v. Red degree is defined as the number of edges not propagated to all v's neighbours. Size n

    int *i_First;      // index, in i_Target, where the inedges of vertex v starts.
    int *i_Target;     // second enpoint for each edge
    int *i_degree;     // current in-degree for each vertex v
    int *i_red_degree; // current red in-degree for each vertex v. Red degree is defined as the number of edges not propagated to all v's neighbours

    int gamma, k;
    float phi;

    int *queue;
    int *visited;
    int bfs_timestamp;

    // METHODS
    void process_edges(int *edges);           // utility method used into create method
    void init(int n, int m, bool isDirected); // dopo lo facciamo diventare il costruttore

    void propagate(int u);

public:
    T *balls;
    ~Graph_csr();

    Graph_csr(int N, int M, bool isDirected = false, int k = 0, float phi = 0.0); // constructor
    Graph_csr(int N, int M, bool isDirected, int k, float phi, int n_hashes, Hash<int> **hash_functions);

    void insert_edge(int u, int v); // insert edge from u to v

    bool check_edge(int u, int v);

    int bfs_2(int u);

    int getN() const;

    void setN(int n);

    int getM() const;

    void setM(int m);

    void setThreshold(float phi);

    template <typename U = T, typename std::enable_if<!std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_file(std::string filename, bool isDirected, int k = 0, float phi = 0.0);

    template <typename U = T, typename std::enable_if<!std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_edges(int *edges, int n, int m, bool isDirected, int k = 0, float phi = 0.0);

    template <typename U = T, typename std::enable_if<std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_file(std::string filename, bool isDirected, int k = 0, float phi = 0.0, int n_hashes = 0, Hash<int> **hash_functions = NULL);

    template <typename U = T, typename std::enable_if<std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_edges(int *edges, int n, int m, bool isDirected, int k = 0, float phi = 0.0, int n_hashes = 0, Hash<int> **hash_functions = NULL);

    void update(int u, int v);

    void print_graph();
};

#endif // DECREMENTALSCC_GRAPH_CSR_H
