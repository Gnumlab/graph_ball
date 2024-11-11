//
// Created by balmung on 08/03/19.
//

#ifndef DECREMENTALSCC_GRAPH_H
#define DECREMENTALSCC_GRAPH_H

#include <string>

#define NIL -1

class Graph {

private:
    int n, m;           //number of vertices and number of edges
    int first_free_index;       //first free position in o/i_First aka first index of a vertex with no edges that can be used in future.
                                //Note: if first_free_index is equal to n then there is no space for more vertices

    int *o_First;         // position of first out-neighbor
    int *o_Target;        // stores out-neighbors
    int *o_Next;          // position of next out-neighbor
    int o_current_pos;    // next available position in o_Target

    int *i_First;       // position of first in-neighbor
    int *i_Target;      // stores in-neighbors
    int *i_Next;        // position of next in-neighbor
    int i_current_pos;  // next available position in i_Target


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
    inline int delete_in_out_edge(int u, int v, int* First, int* Target, int* Next);



public:
    ~Graph();

    void create(int *input, int n, int m);      // create a graph from data read from file
    void init(int N, int M);         //initialization of arrays' space and variables

    int is_out_adjacent_empty(int vertex);          // return true if vertex's out adjacent list is empty
    int is_in_adjacent_empty(int vertex);           // return true if vertex's in adjacent list is empty

    void insert_edge(int u, int v);       //insert edge from u to v
    int delete_edge(int u, int v);       //delete edge (u, v)

    void insert_edge_position(int u, int v, int position);      // insert edge (u, v) into Next[position]

    int check_edge(int u,int v);
    
    void change_first_endpoint(int old_u, int new_u, int v);        // remove edge (old_u, v) and insert (new_u, v)
    void change_second_endpoint(int u, int old_v, int new_v);       // replace edge (u, old_v) with (u, new_v)


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
    

    void delete_all_edges(int v);
    void generate(std::string filename, int direction);
};


#endif //DECREMENTALSCC_GRAPH_H
