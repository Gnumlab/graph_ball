//
// Created by balmung on 08/03/19.
//

#ifndef DECREMENTALSCC_GRAPH_CSR_H
#define DECREMENTALSCC_GRAPH_CSR_H

#include <string>

#define NIL -1

class Graph_csr {

private:
    int n, m;           //number of vertices and number of edges
    
    
    
    int *o_First;      // index, in o_Target, where the outedges of vertex v starts. Size n
    int *o_Target;     // second enpoint for each edge. Size m
    int *o_degree;     // current out-degree for each vertex v. Size n
    int *o_red_degree; // current red out-degree for each vertex v. Red degree is defined as the number of edges not propagated to all v's neighbours. Size n
    
    int *i_First;      // index, in i_Target, where the inedges of vertex v starts.
    int *i_Target;     // second enpoint for each edge
    int *i_degree;     // current in-degree for each vertex v
    int *i_red_degree; // current red in-degree for each vertex v. Red degree is defined as the number of edges not propagated to all v's neighbours
    

    void *sketches; //TODO: add actual sketch implementation
    int epsilon, gamma, k;
    

    int *queue;
    int *visited;
    int bfs_number;
    
    std::pair<int, int> *degree;
    

    int *gamma_values;
    int max_gamma = 0;
    int over_threshold = 0;

    int count;

    //METHODS
    void processInput(int *input);      //insert all the edges stored in input

    void propagate(int u);



public:
    ~Graph_csr();

    void create(int *input, int n, int m);      // create a graph from data read from file
    void init(int N, int M);         //initialization of arrays' space and variables

    int is_out_adjacent_empty(int vertex);          // return true if vertex's out adjacent list is empty
    int is_in_adjacent_empty(int vertex);           // return true if vertex's in adjacent list is empty

    void insert_edge(int u, int v);       //insert edge from u to v
    int delete_edge(int u, int v);       //delete edge (u, v)

    int delete_from_target(int v, int first, int *Target, int end);

    int check_edge(int u,int v);

    void bfs_2(int s);
    
    
    void print_top_balls(int n_balls);

    void print_graph();


    int getN() const;

    void setN(int n);

    int getM() const;

    void setM(int m);


    void compute_gamma(int start, int end);
    void compute_vertex_gamma(int v);
    int bfs_gamma(int s, int *queue, char *visited, int *visited_vertices, int *indegrees, int *outdegrees, int *level);
    


    void generate(std::string filename, int direction);
    
    
    void update(int u, int v);
};


#endif //DECREMENTALSCC_GRAPH_CSR_H
