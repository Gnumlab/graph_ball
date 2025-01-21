#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>
#include <random>
#include <fstream>

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
int *read_edges(std::string filename, int *n, int *m)
{
    std::ifstream file(filename);

    file >> *n;
    file >> *m;

    int *edges = new int[2 * (*m)];
    int from, to;
    int i = 0;

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
