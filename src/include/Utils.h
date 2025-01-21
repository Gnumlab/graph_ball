#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

int* read_edges(std::string filename, int *n, int *m);
void permute_edges(int* edges, int size);
std::vector<std::pair<int, int>> read_ball_sizes(std::string filename);

#endif 
