import sys
import os
from collections import defaultdict

if len(sys.argv) < 2:
    exit(0)

FILE_NAME = sys.argv[1]
IS_DIRECTED = False if len(sys.argv) < 3 else sys.argv[2] == "--directed"

# read file
lines = [
    tuple(map(int, (u, v, t)))
    for u, v, o, t in map(lambda x: x.split(), open(FILE_NAME, "r").readlines())
    if int(o) == 1
]

# sort by timestamp
lines.sort(key=lambda x: x[2])

# normalize node index to start from 0
id = 0
node_map = dict()
for u, v, _ in lines:
    if u not in node_map:
        node_map[u] = id
        id += 1
    if v not in node_map:
        node_map[v] = id
        id += 1

for i, (u, v, t) in enumerate(lines):
    lines[i] = (node_map[u], node_map[v], t)

# remove duplicate edges
d = defaultdict(int)

_edges = []
for u, v, _ in lines:
    if d[(u, v)] == 0:
        _edges.append((u, v))
        d[(u, v)] += 1
        d[(v, u)] += int(not IS_DIRECTED)


m = len(_edges)
n = len(set([u for u, _ in _edges] + [v for _, v in _edges]))

max1 = max(_edges, key=lambda x: x[0])
max2 = max(_edges, key=lambda x: x[1])

print(f"n: {n}, m: {m}")
# print(f"max1: {max1}, max2: {max2}")
assert n == max(max1[0], max2[1]) + 1

# write to file
with open(f"data/{os.path.basename(FILE_NAME)}", "w") as f:
    f.write(f"{n}\n{m}\n")
    for u, v in _edges:
        f.write(f"{u} {v}\n")
