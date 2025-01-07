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