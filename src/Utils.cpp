#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>
#include <random>
#include <fstream>
#include <unordered_map>
#include "include/Utils.h"

/**
 * Read the sequence of edges from the given file.
 * The first line of the file should be the number of nodes of the graph.
 * The second line of the file should be the number of edges of the graph.
 * The following lines should contain the sequence of edge insertions.
 * Each line should contain two integers, the first integer is the node id of the source node and the second integer is the node id of the target node.
 * @param filename The name of the file to read the sequence of edges from.
 * @param n A pointer to the number of nodes of the graph.
 * @param m A pointer to the number of edges of the graph.
 * @return An array of integers, where each pair of integers represents an edge.
 */
uint32_t *read_edges(std::string filename, uint32_t *n, uint64_t *m)
{
    std::ifstream file(filename);

    file >> *n;
    file >> *m;

    uint32_t *edges = new uint32_t[2 * (*m)];
    uint32_t from, to;
    uint64_t i = 0;

    while (file >> from)
    {
        file >> to;
        edges[i++] = from;
        edges[i++] = to;
    }

    file.clear();
    file.close();

    return edges;
}

/**
 * Read the ball sizes from the given file.
 * The file should contain a list of pairs of integers, where the first integer is the node id and the second integer is the size of its ball of radius 2.
 * @param filename The name of the file to read the ball sizes from.
 * @return A vector of pairs of integers, where the first integer is the node id and the second integer is the size of its ball of radius 2.
 */
std::vector<std::pair<int, int>> read_ball_sizes(std::string filename)
{
    std::ifstream file(filename);

    std::vector<std::pair<int, int>> ball_sizes;
    int ball, size;

    while (file >> ball)
    {
        file >> size;
        ball_sizes.push_back({ball, size});
    }

    file.clear();
    file.close();

    return ball_sizes;
}

/**
 * Read the minhash signatures from the given file.
 * The file should contain a list of integers per line, where the first integer is the node id and the following integers are the minhash signatures.
 * @param filename The name of the file to read the minhash signatures from.
 * @param sig_size The size of the minhash signatures.
 * @return A map of integers to arrays of integers, where the key is the node id and the value is the array of minhash signatures.
 */
std::vector<std::pair<int, uint32_t *>> read_signatures(std::string filename, int sig_size)
{
    std::ifstream file(filename);

    std::vector<std::pair<int, uint32_t *>> signatures;
    int node;

    while (file >> node)
    {
        uint32_t *signature = new uint32_t[sig_size];
        for (int i = 0; i < sig_size; i++)
        {
            file >> signature[i];
        }
        signatures.push_back({node, signature});
    }

    return signatures;
}

/**
 * This function reads the balls of radius 2 from the given file.
 * The file should contain a ball per line, where the first integer is the node id, the second is the size of the ball and the following integers are the nodes in the ball.
 *
 * Example:
 * 1 3 1 3 5
 * 2 4 2 4 6 7
 *
 * @param filename The name of the file to read the balls from.
 * @return A map of integers to sets of integers, where the key is the node id and the value is the set of nodes in the ball.
 */
std::unordered_map<int, std::unordered_set<int>> read_balls(std::string filename)
{
    std::ifstream file(filename);
    std::unordered_map<int, std::unordered_set<int>> balls;

    int node;
    int size;

    while (file >> node)
    {
        file >> size;

        int neighbor;
        std::unordered_set<int> ball;

        for (int i = 0; i < size; i++)
        {
            file >> neighbor;
            ball.insert(neighbor);
        }

        balls[node] = ball;
    }

    file.clear();
    file.close();
    return balls;
}

/* premute the edge set edges. The edge (u, v) in edges is stored as two consecutive integers
 * The permutation is executed in a selection sort manner: suppose the first i position permuted, select at random the next edges to put in position i+1.
 * The edge is taken in the unpermuted part of the array (last size - i elements)
 */
void permute_edges(int *edges, int size)
{
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < size / 2; i++)
    {
        // Calculate the number of unpermuted edges
        int remaining = size / 2 - i;

        // Select a random index from the unpermuted part
        int random_index = i + rand() % remaining;

        // Swap the current edge with the randomly selected edge
        int temp_u = edges[i * 2];
        int temp_v = edges[i * 2 + 1];

        edges[i * 2] = edges[random_index * 2];
        edges[i * 2 + 1] = edges[random_index * 2 + 1];

        edges[random_index * 2] = temp_u;
        edges[random_index * 2 + 1] = temp_v;
    }
}
