#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>
#include <random>
#include <fstream>

/* read graph from input file */
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


/* premute the edge set edges. The edge (u, v) in edges is stored as two consecutive integers
 * The permutation is executed in a selection sort manner: suppose the first i position permuted, select at random the next edges to put in position i+1.
 * The edge is taken in the unpermuted part of the array (last size - i elements)
*/
void permute_edges(int* edges, int size) {
    // Seed the random number generator
    srand(time(NULL));

    for (int i = 0; i < size / 2; i++) {
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
