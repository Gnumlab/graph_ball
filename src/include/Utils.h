//
// Created by balmung on 11/03/19.
//

#ifndef DECREMENTALSCC_UTILS_H
#define DECREMENTALSCC_UTILS_H

#include <string>

// int* read_Graph(const char *file, int *n, int *m);
int* read_Graph(std::string filename, int *n, int *m, bool isDirected);

int *ErdosReniyGraph(int size, double prob, int seed, int *number_edges);
int compare_scc(int *scc0, int *scc1, int n);
void writeGraph(int *g, int n, int m, std::string filename);

// Comparator function for sorting based on value
bool comparePair(const std::pair<int, int> a, const std::pair<int, int> b);


#endif //DECREMENTALSCC_UTILS_H
