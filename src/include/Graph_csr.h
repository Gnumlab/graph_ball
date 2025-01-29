#ifndef GRAPH_CSR_H
#define GRAPH_CSR_H

#include <string>
#include "../Hash.cpp"
#include "MinHashBall.h"

template <class T>
class Graph_csr
{

private:
    /// Number of vertices in the graph
    int n;

    /// Number of edges in the graph
    int m;

    /// Boolean value indicating whether the graph is directed or not
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

    void process_edges(int *edges);

    int propagate(int u);


public:
    T *balls;

    /**
     * This is the default destructor of the Graph_csr class.
     */
    ~Graph_csr();

    /**
     * This is the default constructor of the Graph_csr class.
     * @param N: number of vertices in the graph.
     * @param M: number of edges in the graph.
     * @param isDirected: boolean value indicating whether the graph is directed or not.
     * @param k: number of nodes to sample when updating the graph.
     * @param phi: threshold value for the red degree of a vertex.
     */
    Graph_csr(int N, int M, bool isDirected = false, int k = 0, float phi = 0.0);

    /**
     * This is the constructor of the Graph_csr class for MinHashBall objects.
     * @param N: number of vertices in the graph.
     * @param M: number of edges in the graph.
     * @param isDirected: boolean value indicating whether the graph is directed or not.
     * @param k: number of nodes to sample when updating the graph.
     * @param phi: threshold value for the red degree of a vertex.
     * @param n_hashes: number of hash functions to use for the MinHashBall objects.
     * @param hash_functions: array of hash functions to use for the MinHashBall objects.
     */
    Graph_csr(int N, int M, bool isDirected, int k, float phi, int n_hashes, Hash<uint32_t> **hash_functions);

    bool check_edge(int u, int v);

    /**
     * This method performs a breadth-first search on the graph starting from the given vertex u and stops at distance 2.
     * @param u: vertex to start the breadth-first search from.
     * @return: size of the subgraph reachable from vertex u within distance 2.
     */
    int bfs_2(int u);

    /**
     * This method returns the ball of radius 2 of the given vertex u.
     * @param u: vertex for which to return the ball of radius 2.
     * @return: vector containing the vertices in the ball of radius 2 of vertex u.
     */
    std::vector<int> ball_2(int u);

    /**
     * This method returns the number of vertices in the graph.
     */
    int getN() const;

    /**
     * This method sets the number of vertices in the graph.
     * @param n: number of vertices in the graph.
     */
    void setN(int n);

    /**
     * This method returns the number of edges in the graph.
     */
    int getM() const;

    /**
     * This method sets the number of edges in the graph.
     * @param m: number of edges in the graph.
     */
    void setM(int m);

    /**
     * This method sets the threshold value for the red degree of each vertex in the graph.
     * The threshold value is used to determine when to propagate the red edges.
     * @see propagate
     * @param phi: threshold value for the red degree of a vertex.
     */
    void setThreshold(float phi);

    void setK(int k);

    /**
     * This method inserts the edge (u, v) into the graph.
     * @param u: first endpoint of the edge to insert.
     * @param v: second endpoint of the edge to insert.
     * @return: void
     */
    void insert_edge(int u, int v);

    /**
     * This method reads the graph from the given file and returns a Graph_csr object.
     * @param filename: name of the file containing the graph.
     * @param isDirected: boolean value indicating whether the graph is directed or not.
     * @param k: number of nodes to sample when updating the graph.
     * @param phi: threshold value for the red degree of a vertex.
     * @return: Graph_csr object representing the graph read from the file.
     */
    template <typename U = T, typename std::enable_if<!std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_file(std::string filename, bool isDirected, int k = 0, float phi = 0.0);

    /**
     * This method reads the graph from the given edges and returns a Graph_csr object.
     * @param edges: array containing the edges of the graph.
     * @param n: number of vertices in the graph.
     * @param m: number of edges in the graph.
     * @param isDirected: boolean value indicating whether the graph is directed or not.
     * @param k: number of nodes to sample when updating the graph.
     * @param phi: threshold value for the red degree of a vertex.
     * @return: Graph_csr object representing the graph read from the edges.
     */
    template <typename U = T, typename std::enable_if<!std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_edges(int *edges, int n, int m, bool isDirected, int k = 0, float phi = 0.0);

    /**
     * This method reads the graph from the given file and returns a Graph_csr<MinHashBall> object.
     * @param filename: name of the file containing the graph.
     * @param isDirected: boolean value indicating whether the graph is directed or not.
     * @param k: number of nodes to sample when updating the graph.
     * @param phi: threshold value for the red degree of a vertex.
     * @param n_hashes: number of hash functions to use for the MinHashBall objects.
     * @param hash_functions: array of hash functions to use for the MinHashBall objects.
     * @return: Graph_csr object representing the graph read from the file.
     */
    template <typename U = T, typename std::enable_if<std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_file(std::string filename, bool isDirected, int k = 0, float phi = 0.0, int n_hashes = 0, Hash<uint32_t> **hash_functions = NULL);

    /**
     * This method reads the graph from the given edges and returns a Graph_csr<MinHashBall> object.
     * @param edges: array containing the edges of the graph.
     * @param n: number of vertices in the graph.
     * @param m: number of edges in the graph.
     * @param isDirected: boolean value indicating whether the graph is directed or not.
     * @param k: number of nodes to sample when updating the graph.
     * @param phi: threshold value for the red degree of a vertex.
     * @param n_hashes: number of hash functions to use for the MinHashBall objects.
     * @param hash_functions: array of hash functions to use for the MinHashBall objects.
     * @return: Graph_csr object representing the graph read from the edges.
     */
    template <typename U = T, typename std::enable_if<std::is_same<U, MinHashBall>::value, int>::type = 0>
    static Graph_csr<T> *from_edges(int *edges, int n, int m, bool isDirected, int k = 0, float phi = 0.0, int n_hashes = 0, Hash<uint32_t> **hash_functions = NULL);

    /**
     * This method computes minhash signatures for the ball of radius 2 of the given vertex u.
     * It performs a breadth-first search starting from vertex u and stops at distance 2.
     * The minhash signatures are computed using the given hash functions.
     * @param u: vertex for which to compute the minhash signatures.
     * @param n_hashes: number of hash functions to use.
     * @param hash_functions: array of hash functions to use.
     */
    uint32_t *computeExactMHSignature(int u, int n_hashes, Hash<uint32_t> **hash_functions);

    /**
     * This method computes few-permutation hashing signatures for the ball of radius 2 of the given vertex u.
     * It performs a breadth-first search starting from vertex u and stops at distance 2.
     * The minhash signatures are computed using the given hash functions.
     * @param u: vertex for which to compute the minhash signatures.
     * @param n_hashes: number of hash functions to use.
     * @param hash_functions: array of hash functions to use.
     * @param sig_size: size of the signature to compute.
     */
    uint32_t *computeExactOPHSignature(int u, int n_hashes, Hash<uint32_t> **hash_functions, int sig_size);

    /**
     * This method updates the graph by adding the edge (u, v) to the graph.
     * @param u: first endpoint of the edge to add.
     * @param v: second endpoint of the edge to add.
     */
    int update(int u, int v);

    /**
     * This method prints the graph to the standard output.
     * @param doPrintBall: boolean value indicating whether to print the balls of each vertex.
     */
    void print_graph(bool doPrintBall = false);

    /**
     * This method fills the graph with all the edges red from the file.
     * The complexity of this method is O(n), since it set the degree of each vertex to the number of edges incident to it.
     */
    void fill_graph();

    /**
     * This method flushes the graph by removing all the edges from the graph.
     * The complexity of this method is O(n), since it set the degree of each vertex to 0.
     */
    void flush_graph();

    void print_vertex(int i, bool doPrintBall = false);
};

#endif
