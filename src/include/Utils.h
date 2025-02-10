#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

uint32_t *read_edges(std::string filename, uint32_t *n, uint64_t *m);
void permute_edges(int *edges, int size);
std::vector<std::pair<uint32_t, uint32_t>> read_ball_sizes(std::string filename);
std::vector<std::pair<uint32_t, uint32_t *>> read_signatures(std::string filename, int sig_size);
std::unordered_map<uint32_t, std::unordered_set<uint32_t>> read_balls(std::string filename);

#endif
