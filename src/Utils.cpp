//
// Created by balmung on 11/03/19.
//

#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>
#include <random>
#include <fstream>

/* read graph from input file */
int *read_Graph(std::string filename, int *n, int *m, bool isDirected)
{
    // std::cerr << "File " + filename + " reading started\n";

    std::ifstream file(filename);

    file >> *n;
    file >> *m;
    *m = *m * (2 - isDirected); // if direction is 0 then graph is undirected and we save an edge twice

    int *edges = new int[2 * (*m)];
    int from, to, avoid;
    int i = 0;

    while (file >> from)
    {
        file >> to;
        edges[i++] = from;
        edges[i++] = to;
        if (!isDirected)
        { // graph is undirected then must insert both edges (u, v) and (v, u)
            edges[i++] = to;
            edges[i++] = from;
        }
        //        file >> avoid;
    }

    file.clear();
    file.close();
    // std::cerr << "File " + filename + " read, ";
    // fprintf(stderr, "n = %d m = %d.\n", *n, *m);

    return edges;
}