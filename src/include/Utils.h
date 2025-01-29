#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

int* read_edges(std::string filename, int *n, int *m);
void permute_edges(int* edges, int size);
std::vector<std::pair<int, int>> read_ball_sizes(std::string filename);
std::vector<std::pair<int, uint32_t*>> read_signatures(std::string filename, int sig_size);
std::unordered_map<int, std::unordered_set<int>> read_balls(std::string filename);

#endif 
