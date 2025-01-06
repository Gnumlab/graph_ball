//
// Created by balmung on 08/03/19.
//

#include <iostream>
#include <algorithm>

#include "include/Utils.h"
#include "include/Graph_csr.h"
#include "include/MinHashBall.h"
#include "Hash.cpp"

template <class T>
Graph_csr<T>::~Graph_csr()
{
    delete[] o_First;
    delete[] o_Target;
    delete[] o_degree;
    delete[] o_red_degree;

    if (this->directed)
    {
        delete[] i_First;
        delete[] i_Target;
        delete[] i_degree;
        delete[] i_red_degree;
    }

    delete[] queue;
    delete[] visited;
    delete[] balls;
}

// constructor
template <class T>
Graph_csr<T>::Graph_csr(int N, int M, bool isDirected, int k, float phi)
{
    this->n = N;
    this->m = M;
    this->directed = isDirected;
    this->k = k;
    this->phi = phi;

    this->o_First = new int[N];
    this->o_Target = new int[M];
    this->o_degree = new int[N];
    this->o_red_degree = new int[N];

    if (isDirected)
    {
        this->i_First = new int[N];
        this->i_Target = new int[M];
        this->i_degree = new int[N];
        this->i_red_degree = new int[N];
    }

    this->balls = new T[N];
    for (int i = 0; i < n; i++)
    {
        this->balls[i].insert(i);
        this->o_First[i] = 0;
        this->o_degree[i] = 0;
        this->o_red_degree[i] = 0;

        if (isDirected)
        {
            this->i_First[i] = 0;
            this->i_degree[i] = 0;
            this->i_red_degree[i] = 0;
        }
    }

    this->queue = new int[N];
    this->visited = new int[N];

    this->bfs_timestamp = 0;
}

template <class MinHashBall>
Graph_csr<MinHashBall>::Graph_csr(int N, int M, bool isDirected, int k, float phi, int n_hashes, Hash<int> **hash_functions)
    : Graph_csr<MinHashBall>(N, M, isDirected, k, phi)
{
    for (int i = 0; i < this->n; i++)
    {
        this->balls[i] = MinHashBall(hash_functions, n_hashes);
    }
}

template <class T>
void Graph_csr<T>::process_edges(int *edges)
{
    int *max_indegree = new int[n];  // maximum in degree for each vertex; used to initialize i_First
    int *max_outdegree = new int[n]; // maximum out degree for each vertex; used to initialize o_First

    for (int i = 0; i < this->n; i++)
    {
        max_indegree[i] = 0;
        max_outdegree[i] = 0;
    }

    for (int i = 0; i < 2 * this->m; i += 2)
    {
        max_outdegree[edges[i]]++;
        max_indegree[edges[i + 1]]++;
    }

    // init i_First and o_First; no need to init the first vertex since it is already initialise
    for (int i = 1; i < this->n; i++)
    {
        this->o_First[i] = this->o_First[i - 1] + max_outdegree[i - 1];
        if (this->directed)
            this->i_First[i] = this->i_First[i - 1] + max_indegree[i - 1];
    }

    for (int i = 0; i < 2 * this->m; i += 2)
    {
        int u = edges[i];
        int v = edges[i + 1];
        if (!this->check_edge(u, v))
            this->insert_edge(u, v);
    }

    for (int i = 0; i < this->n; i++)
    {
        this->o_degree[i] = 0;
        if (this->directed)
            this->i_degree[i] = 0;
    }

    delete[] max_indegree;
    delete[] max_outdegree;
}

template <class T>
int Graph_csr<T>::getN() const
{
    return this->n;
}

template <class T>
void Graph_csr<T>::setN(int n)
{
    this->n = n;
}

template <class T>
int Graph_csr<T>::getM() const
{
    if (this->directed)
        return this->m;
    else
        return this->m / 2;
}

template <class T>
void Graph_csr<T>::setM(int m)
{
    this->m = m;
}

template <class T>
void Graph_csr<T>::insert_edge(int u, int v)
{
    // store (u, v) as out edge
    this->o_Target[this->o_First[u] + this->o_degree[u]] = v;
    this->o_degree[u]++;

    if (this->directed)
    {
        // store (u, v) as in edge
        this->i_Target[this->i_First[v] + this->i_degree[v]] = u;
        this->i_degree[v]++;
    }
    else
    {
        this->o_Target[this->o_First[v] + this->o_degree[v]] = u;
        this->o_degree[v]++;
    }
}

template <class T>
bool Graph_csr<T>::check_edge(int u, int v)
{
    /** che if edge (u, v) already exists. Return the 1 if (u, v) is in G, 0 otherwise */
    for (int next_u = o_First[u]; next_u < o_First[u] + o_degree[u]; next_u++)
        if (o_Target[next_u] == v)
            return true;
    return false;
}

// implemntation for any class T that is not MinHashBall
template <class T>
template <typename U, typename std::enable_if<!std::is_same<U, MinHashBall>::value, int>::type>
Graph_csr<T> *Graph_csr<T>::from_file(std::string filename, bool isDirected, int k, float phi)
{
    int n, m;
    int *edges = read_Graph(filename, &n, &m, isDirected);
    return Graph_csr<T>::from_edges(edges, n, m, isDirected, k, phi);
}

template <class T>
template <typename U, typename std::enable_if<!std::is_same<U, MinHashBall>::value, int>::type>
Graph_csr<T> *Graph_csr<T>::from_edges(int *edges, int n, int m, bool isDirected, int k, float phi)
{
    Graph_csr<T> *graph = new Graph_csr<T>(n, m, isDirected, k, phi);
    graph->process_edges(edges);
    return graph;
}

// implemntation for MinHashBall
template <class T>
template <typename U, typename std::enable_if<std::is_same<U, MinHashBall>::value, int>::type>
Graph_csr<T> *Graph_csr<T>::from_file(std::string filename, bool isDirected, int k, float phi, int n_hashes, Hash<int> **hash_functions)
{
    int n, m;
    int *edges = read_Graph(filename, &n, &m, isDirected);
    return Graph_csr<T>::from_edges(edges, n, m, isDirected, k, phi, n_hashes, hash_functions);
}

template <class T>
template <typename U, typename std::enable_if<std::is_same<U, MinHashBall>::value, int>::type>
Graph_csr<T> *Graph_csr<T>::from_edges(int *edges, int n, int m, bool isDirected, int k, float phi, int n_hashes, Hash<int> **hash_functions)
{
    Graph_csr<T> *graph = new Graph_csr<T>(n, m, isDirected, k, phi, n_hashes, hash_functions);
    graph->process_edges(edges);
    return graph;
}

template <class T>
int Graph_csr<T>::bfs_2(int u)
{
    int size = o_degree[u] + 1;
    int v, w;
    bfs_timestamp++;

    visited[u] = bfs_timestamp;

    for (int i = o_First[u]; i < o_First[u] + o_degree[u]; i++)
        visited[o_Target[i]] = bfs_timestamp;

    for (int i = o_First[u]; i < o_First[u] + o_degree[u]; i++)
    {
        v = o_Target[i];

        for (int j = o_First[v]; j < o_First[v] + o_degree[v]; j++)
        {
            w = o_Target[j];
            if (visited[w] != bfs_timestamp)
            {
                visited[w] = bfs_timestamp;
                size++;
            }
        }
    }

    return size;
}

/**
 * This method is used to propagate the vertices of the ball of radius 1 of the given vertex u to the ball of radius 2 of all its neighbours.
 * @param u: vertex whose ball of radius 1 is to be propagated to the ball of radius 2 of all its neighbours.
 * @return: void
 */
template <class T>
void Graph_csr<T>::propagate(int u)
{
    for (int i = this->o_First[u]; i < this->o_First[u] + this->o_degree[u]; i++)
    {
        int v = this->o_Target[i];
        this->balls[v].push(&this->balls[u]);
    }
}

template <class T>
void Graph_csr<T>::setThreshold(float phi)
{
    this->phi = phi;
}

template <class T>
void Graph_csr<T>::update(int u, int v)
{
    // increment the red degree of u
    this->o_red_degree[u]++;

    // increment the degree of u
    this->o_degree[u]++;

    // insert v into the ball of u
    this->balls[u].insert(v);

    // push the ball of radius 1 of v to the ball of radius 2 of u
    this->balls[u].push(&this->balls[v]);

    // check if the red degree of u is greater than the threshold
    float threshold = this->phi * (this->o_degree[u] - this->o_red_degree[u]);
    if (this->o_red_degree[u] >= threshold)
    {
        // reset the red degree of u
        this->o_red_degree[u] = 0;
        // propagate the verites of u's ball of radius 1 to the ball of radius 2 of all its neighbours
        this->propagate(u);
    }
    else
    {
        for (int i = 0; i < this->k; i++)
        {
            int rand_index = this->o_First[u] + (rand() % this->o_degree[u]);
            int x = this->o_Target[rand_index];
            this->balls[x].push(&this->balls[u]);
        }
    }

    if (!this->directed)
    {
        this->o_red_degree[v]++;
        this->o_degree[v]++;
        this->balls[v].insert(u);
        this->balls[v].push(&this->balls[u]);

        threshold = this->phi * (this->o_degree[v] - this->o_red_degree[v]);
        if (this->o_red_degree[v] >= threshold)
        {
            this->o_red_degree[v] = 0;
            this->propagate(v);
        }
        else
        {
            for (int i = 0; i < k; i++)
            {
                int rand_index = this->o_First[v] + (rand() % this->o_degree[v]);
                int x = o_Target[rand_index];
                this->balls[x].push(&this->balls[v]);
            }
        }
    }
}

template <class T>
void Graph_csr<T>::print_graph(bool doPrintBall)
{
    printf("nv=%d, ne=%d, direct=%d, phi=%.3f, k=%d\n", this->n, this->getM(), this->directed, this->phi, this->k);
    for (int i = 0; i < this->n; i++)
    {
        int start = this->o_First[i];
        int degree = this->o_degree[i];
        int red_degree = this->o_red_degree[i];
        int end = i < this->n - 1 ? this->o_First[i + 1] : this->m;
        printf("[%d, d=%d, \033[31mrd=%d\033[0m]:\t", i, degree, red_degree);

        for (int j = start; j < end; j++)
        {
            if (j < start + degree - red_degree)
                printf("\033[37m%d\033[0m ", this->o_Target[j]);
            else if (j < start + degree)
                printf("\033[31m%d\033[0m ", this->o_Target[j]);
            else
                printf("\033[38;5;254;48;5;245m%d \033[0m", this->o_Target[j]);
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    if (doPrintBall)
    {
        std::cout << "Balls:" << std::endl;
        for (int i = 0; i < this->n; i++)
        {
            printf("[%d], size=%d, real_size=%d\n", i, this->balls[i].size(), this->bfs_2(i));
            this->balls[i].print();
        }
        std::cout << std::endl;
    }
}