#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

int* read_edges(std::string filename, int *n, int *m);
std::vector<int> *topKBall2(std::string filename, int k, bool isDirected);

#endif 