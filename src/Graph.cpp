//
// Created by balmung on 08/03/19.
//

#include <iostream>
#include <algorithm>
//#include <stack>

#include "include/Utils.h"
#include "include/Graph.h"


#define MAX(a,b) (((a)>(b))?(a):(b))

Graph::~Graph(){
    delete [] o_First;
}

void Graph::create(int *input, int n, int m){
    /** create a graph from file previously read. Initialize all the data structures and attributes.
     * @param input array with all the edges. Starting from 0, an edge is stored (u, v) is stored in position i and i+1
     * @param n number of vertices: note that vertices are numbers from 1 to n
     * @param m number of edges
     */

    init(n+1, m+1);
    processInput(input);

}

void Graph::init(int N, int M) {
    /** initialize all the attributes and allocate the space for storing the graph
     * @param N number of vertices
     * @param M number of edges
     */

    n = N; m = M;
    first_free_index = 0;
    o_current_pos = i_current_pos = 1;
    int *buffer = new int[14 * (N) + 5 * M];       //buffer to store vertices and edges, both in and out

    o_First = buffer;           //n vertices        From 0 to N-1
    o_Target = buffer + N;      //m out-edges       From N to M + N -1
    o_Next = buffer + (N + M);  //m out-edges       From M + N to 2M + N -1



    i_First = buffer + (N + 2 * M);
    i_Target = buffer + (2 * N + 2 * M);
    i_Next = buffer + (2 * N + 3 * M);

    for(int i = n; i >= 0; i--) {
        o_First[i] = -1;
        i_First[i] = -1;
    }

    
    queue = buffer + (2 * N + 4 * M);
    visited = buffer + (3 * N + 4 * M);
    
    degree = new std::pair<int, int>[N];
    for (int i = 0; i < N; i++){
        degree[i].first = i;
        degree[i].second = 0;
    } 
    
    gamma_values = new int[N];


    bfs_number = 0;
    over_threshold = 0;
    max_gamma = 0;
    count = 0;
}


void Graph::processInput(int *input){
    /** Insert all the edges stored in input. The data structures are supposed initialize.
     * @param input array with all the edges. Starting from 0, an edge is stored (u, v) is stored in position i and i+1
     */

    for(int i = 0; i < 2 * (m-1); i += 2){
//        std::cout << "" << input[i] << " " << input[i+1] << "\n";
        if(input[i] > n || input[i+1] > n) continue;
        if(!check_edge(input[i], input[i+1])) insert_edge(input[i], input[i+1]);
        else count++;
//        first_free_index = std::max(first_free_index, input[i] + 1);
    }
//    std::cout << "Graph::print_graph; edge " << first_free_index << "\n";

    //fprintf(stderr, "elses %d / %d\n", count, m);

}


int Graph::getN() const {
    return n;
}

void Graph::setN(int n) {
    Graph::n = n;
}

int Graph::getM() const {
    return m;
}

void Graph::setM(int m) {
    Graph::m = m;
}





int Graph::is_out_adjacent_empty(int vertex){
    /** return true if vertex's out-adjacent list is empty, false otherwise
     * @param vertex is the vertex to check if it has out going edges
     * @return 1 if vertex has no out going edges, 0 otherwise
     */

    return o_First[vertex] < 0;
}

int Graph::is_in_adjacent_empty(int vertex){
    /** return true if vertex's in-adjacent list is empty, false otherwise
     * @param vertex is the vertex to check if it has incoming edges
     * @return 1 if vertex has no incoming edges, 0 otherwise
     */

    return i_First[vertex] < 0;
}

void Graph::insert_edge(int u,int v){
/** insert edge (u, v) */

    //store (u, v) as out edge
    this->insert_edge_position(u, v, o_current_pos++);
}


inline int Graph::delete_in_out_edge(int u, int v, int* First, int* Target, int* Next){
    /**delete edge (u, v) from either out-edges list or in-edges list
     *@return the freed position in Next
     */


    int i,prev, removed;
    if(Target[First[u]] == v){          //remove the head of adjacent list
        removed = First[u];
        First[u] = Next[First[u]];
    }
    else{
        prev = 0;                     // index of the record previous to the edge to be removed
        i = First[u];
        while(Target[i] != v){      // find where the edge to be removed is
            prev = i;
            i = Next[i];
        }
        removed = i;
        Next[prev] = Next[i];
    }

    return removed;
}


int Graph::delete_edge(int u,int v){
/** delete edge (u, v). Return the position of o_Next and i_Next where the edge (u, v) was stored and now is empty */

    int index = this->delete_in_out_edge(u, v, o_First, o_Target, o_Next);
    this->delete_in_out_edge(v, u, i_First, i_Target, i_Next);

    return index;
}


int Graph::check_edge(int u,int v){
/** che if edge (u, v) already exists. Return the 1 if (u, v) is in G, 0 otherwise */

    for(int next_u = o_First[u]; next_u >= 0; next_u = o_Next[next_u]){
        if(o_Target[next_u] == v) {
            return 1; //found edge (u, v)
        }
    }
    return 0;
}


void Graph::insert_edge_position(int u, int v, int position){
    /** insert edge (u, v) using Next[position] to store it. It is supposed that Next[position] is not used for any other edge
     * @param u is the first endpoint of the edge
     * @param v is the second enpoint of the edge
     * @param position is the index value of o_Next and i_Next where the edge will be stored
     */

  

    o_Target[position] = v;
    o_Next[position] = o_First[u];
    o_First[u] = position;
    i_Target[position] = u;
    i_Next[position] = i_First[v];
    i_First[v] = position;
    
    degree[u].second++;
}



void Graph::change_first_endpoint(int old_u, int new_u, int v){
    /** replace edge (old_u, v) with (new_u, v)
     * @param old_u is the endpoint to be replaced
     * @param new_u is the new first endpoint
     * @param v is the second enpoint
     */

    int new_position = delete_in_out_edge(old_u, v, o_First, o_Target, o_Next);     //new_position is the record I can reuse to store new edge
    // insert new edge at the head of new_u's adjacent list
    o_Next[new_position] = o_First[new_u];
    o_First[new_u] = new_position;

    // update incoming edge of v: need to change only i_Target changing old_u with new_u in the right position which has to be found
    for(int next_v = i_First[v]; next_v >= 0; next_v =i_Next[next_v]){
        if(i_Target[next_v] == old_u) {     // edge (v, old_u) found
            i_Target[next_v] = new_u;       // change old_u with new_u
        }
    }

}

void Graph::change_second_endpoint(int u, int old_v, int new_v){
    /** replace edge (old_u, v) with (new_u, v)
     * @param u is the first enpoint
     * @param old_v is the endpoint to be replaced
     * @param new_v is the new second endpoint
     */


    for(int next_u = o_First[u]; next_u >= 0; next_u = o_Next[next_u]){
        if(o_Target[next_u] == old_v) {
            o_Target[next_u] = new_v;
        }
    }

    int new_position = delete_in_out_edge(old_v, u, i_First, i_Target, i_Next);
    i_Next[new_position] = i_First[new_v];
    i_First[new_v] = new_position;



}






void Graph::print_graph(){
    int u, selected_node;
    int *buffer = this->o_First;

   /* std::cout<<std::endl;
    for(int i = 0; i < n + 2*m; i++){
        if(i == 0)std::cout<<"First\n";
        if(i == n)std::cout<<"Target\n";
        if(i == n+m)std::cout<<"Next\n";
        std::cout<<buffer[i]<<"\n";
    }*/


    for(int i = 1; i < n; i++) {
        u = o_First[i];
        while (u >= 0) {
            selected_node = o_Target[u];
            std::cout << "Graph::print_graph; edge " << i << "->" << selected_node << "    u = " << u << "\n";
            u = o_Next[u];
        }
    }

    /*std::cout << "Graph::print_graph; REVERSE EDGE\n";

    for(int i = n+2*m; i < 2*n + 4*m; i++){
        if(i == n+2*m)std::cout<<"First\n";
        if(i == 2*n+2*m)std::cout<<"Target\n";
        if(i == 2*n+3*m)std::cout<<"Next\n";
        std::cout<<buffer[i]<<"\n";
    }


    for(int i = 1; i < n; i++) {
        u = i_First[i];
        while (u >= 0) {
            selected_node = i_Target[u];
            std::cout << "Graph::print_graph; edge " << i << "->" << selected_node << "    u = " << u << "\n";
            u = i_Next[u];
        }
    }

    std::cout<<std::endl;*/
}



void Graph::delete_all_edges(int v){

    int u,selected_node;

    for(u = o_First[v]; u >= 0; u = o_Next[u]){
        selected_node = o_Target[u];

        delete_edge(v, selected_node);
    }
}



void Graph::generate(std::string filename, int direction){
    std::cerr<<"Reading graph " << filename<<"\n";

    int nV, nE;
    int *graph = read_Graph(filename, &nV, &nE, direction);


    this->create(graph, nV, nE);

 
    //writeGraph(graph, nV, nE, path + filename);
}




void Graph::bfs_2(int s){




    int u, v;
    int head = 0, tail = 0;
    bfs_number++;
    

    
    visited[s] = bfs_number;
    //fprintf(stderr, "%d Visited vertex: %d\n", tid, s);
    
    //queue[tail++] = s;
    
    printf("%d ", s);
    
    for (int i = o_First[s]; i != -1; i = o_Next[i]) {
            v = o_Target[i];
            
            if (visited[v] != bfs_number) {
                visited[v] = bfs_number;
                queue[tail++] = v;
                printf("%d ", v);
                //queue[tail++] = level + 1;
                // add edge to the spanning forest: The edge is treated as directed!
                //fprintf(stderr, "%d edge %d, %d \n", tid, u, v);
                //output[pos_output++] = u;
                //output[pos_output++] = v;
                
            }
    }
    
    while (head != tail) {
        u = queue[head++];
        //level = queue[head++]
        //if (level == r) return;
        for (int i = o_First[u]; i != -1; i = o_Next[i]) {
            v = o_Target[i];
            
            if (visited[v] != bfs_number) {
                visited[v] = bfs_number;
                printf("%d ", v);
                //queue[tail++] = v;
                //queue[tail++] = level + 1;
                // add edge to the spanning forest: The edge is treated as directed!
                //fprintf(stderr, "%d edge %d, %d \n", tid, u, v);
                //output[pos_output++] = u;
                //output[pos_output++] = v;
                
            }
        }
    }
    printf("\n");

}


void Graph::print_top_balls(int n_balls){

    fprintf(stderr, "sorting...\n");
    std::sort(degree, degree + n, comparePair);
    fprintf(stderr, "done\n");
    for (int i = 0; i < n_balls; i++){
        fprintf(stderr, "i = %d, n = %d -> n-1-i = %d\n", i, n, n-1-i);
        bfs_2(degree[n-1-i].first);
    }

}



int Graph::bfs_gamma(int s, int *queue, char *visited, int *visited_vertices, int *indegrees, int *outdegrees, int *level){
// return the number of visited vertices, i.e., size of visited_vertices


    int u, v;
    int head = 0, tail = 0;
    int n_visited = 0;
    int max = 0;

    

    
    visited[s/32]= (1 << (s % 32));
    //fprintf(stderr, "%d Visited vertex: %d\n", tid, s);
    
    //queue[tail++] = s;
    
    //printf("s = %d \n", s);
    
    for (int i = o_First[s]; i != -1; i = o_Next[i]) {
            v = o_Target[i];
            if(v > n)printf("\tbad%d %d\n", v, i);
            //check if neighbor has been visited
            if(!((1 << v%32) & visited[v/32])){ 
                visited[v/32] |= (1 << (v%32));
                visited_vertices[n_visited] = v;
                n_visited++;
                indegrees[v] = 0;  //set the number of in edges in v as 0 since it is has only the tree edge right now
                //outdegrees[v] = 0; outdegree of vertices in L_1(s) is not used, so no need to initialise it
                //visited[v] = bfs_number;
                queue[tail++] = v;
                level[v] = 1; //first level of the bfs tree
                
                
            }
    }
    
    while (head != tail) {
        u = queue[head++];
        //level = queue[head++]
        //if (level == r) return;
        for (int i = o_First[u]; i != -1; i = o_Next[i]) {
            v = o_Target[i];
            if(v == s) continue; // skip edge to the bfs root
            if(v > n) fprintf(stderr,"%d %d\n", v, ((1 << v%32) & visited[v/32]));
            if(!((1 << v%32) & visited[v/32])){ 
                //vertex v never seen before
                visited[v/32] |= (1 << (v%32));
                
                visited_vertices[n_visited] = v;
                n_visited++;
                indegrees[v] = 0;  //set the number of in edges in v as 0 since it is has only the tree edge right now
                outdegrees[v] = 0;
                level[v] = 2; // if we are here, we are in the second level of the bfs tree
                
                
            } else {
              if(level[v] == 1){
                outdegrees[u]++;  //u has a new neighbour in level 1 (that is, v)
                max = MAX(max, outdegrees[v]);
              }
              else{
                indegrees[v]++; // v is in level 2 and it is always reached by a vertex in level 1 (u itself) since I do not explore vertices in level 2
                max = MAX(max, indegrees[v]);
              }
                 // printf("%d %d\n",v, max);
              
            }
            
        }
    }
    printf("%d\n", max);
    max_gamma = MAX(max, max_gamma);
        //printf("\t\t\t%d\n", max);
    if(max > 150) over_threshold++;
        //printf("\t\t\t%d\n", n_visited);
    return n_visited;








}


void Graph::compute_gamma(int start, int end = -1){

  if (end < 0) end = n;
  
  int n_visited;
  int nv = 1 + n/32, v;
  int *queue = new int[n];
  char *visited = new char[nv];
  int *visited_vertices = new int[n];
  int *level = new int[n];      //for each vertex its level in the bfs tree rooted in s. A binaty vector is enough since we are interested ony in level 1 and 2
  int *indegrees = new int[n];  //number of in edges to compute the gamma-okness: used for vertices at level 2 L_2(s)
  int *outdegrees = new int[n]; //number of out edges to compute the gamma-okness: used fr vertices at level 1 L_1(s)
  
  for(int i=0; i<nv; i++) {
    visited[i] = 0;
    indegrees[i] = 0;
  }
  
  
  //run a bfs to compute the gamma-ok value for each vertex between star and end
  for (int i = start; i < end; i++){

    n_visited = bfs_gamma(i, queue, visited, visited_vertices, indegrees, outdegrees, level);
    //restore visited vertices
    //fprintf(stderr,"bfs done %d %d\n", n_visited, i);
    for(int j = 0; j < n_visited; j++){

      v = visited_vertices[j];
             // fprintf(stderr,"bfs done %d %d\n", v, j);
      indegrees[v] = 0;
      visited[v/32] ^= (1 << (v%32));
    }
  
  }
  
  
  
    fprintf(stderr, "Graph's gamma is %d - vertices above the fixed threshold are %d (size n=%d, m=%d)\n", max_gamma, over_threshold, n, m);

}








