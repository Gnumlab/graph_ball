//
// Created by balmung on 08/03/19.
//

#include <iostream>
#include <algorithm>
//#include <stack>

#include "include/Utils.h"
#include "include/Graph_csr.h"


#define MAX(a,b) (((a)>(b))?(a):(b))

Graph_csr::~Graph_csr(){
    delete [] o_First;
}

void Graph_csr::create(int *input, int n, int m){
    /** create a graph from file previously read. Initialize all the data structures and attributes.
     * @param input array with all the edges. Starting from 0, an edge is stored (u, v) is stored in position i and i+1
     * @param n number of vertices: note that vertices are numbers from 1 to n
     * @param m number of edges
     */

    init(n+1, m+1);
    processInput(input);

}

void Graph_csr::init(int N, int M) {
    /** initialize all the attributes and allocate the space for storing the graph
     * @param N number of vertices
     * @param M number of edges
     */

    n = N; m = M;


    int *buffer = new int[8 * (N) + 2 * M];       //buffer to store vertices and edges, both in and out

    o_First = buffer;           //n vertices        From 0 to N-1
    o_Target = buffer + N;     
    o_degree = buffer + (N + M); 
    o_red_degree = buffer + (2 * N + M);


    i_First = buffer + (2 * N + 1 * M);
    i_Target = buffer + (3 * N + 1 * M);
    i_degree = buffer + (3 * N + 2 * M);
    i_red_degree = buffer + (4 * N + 2 * M);

    for(int i = 0; i <= n; i--) {
        o_First[i] = 0;
        i_First[i] = 0;
        o_degree[i] = 0;
        i_degree[i] = 0;
        o_red_degree[i] = 0;
        i_red_degree[i] = 0;
    }

    
    queue = buffer + (3 * N + 2 * M);
    visited = buffer + (4 * N + 2 * M);
    
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


void Graph_csr::processInput(int *input){
    /** Insert all the edges stored in input. The data structures are supposed initialize.
     * @param input array with all the edges. Starting from 0, an edge is stored (u, v) is stored in position i and i+1
     */
     
    int *max_indegree = new int[n]; // maximum in degree for each vertex; used to initialize i_First
    int *max_outdegree = new int[n]; // maximum out degree for each vertex; used to initialize o_First
    int in_count = 0, out_count = 0;
    
    
    for(int i = 0; i <= n; i++) {
        max_indegree[i] = 0;
        max_outdegree[i] = 0;
    }
    
    for(int i = 0; i < 2 * (m-1); i += 2){
        if(input[i] > n || input[i+1] > n) continue;
        max_indegree[input[i+1]]++;
        max_outdegree[input[i]]++;

    }


    // init i_First and o_First; no need to init the first vertex since it is already initialise
     
    for(int i = 2; i <= n; i++) {
        
        in_count += max_indegree[i-1];
        out_count += max_outdegree[i-1];
        i_First[i] = in_count;
        o_First[i] = out_count;
    }


    for(int i = 0; i < 2 * (m-1); i += 2){
//        std::cout << "" << input[i] << " " << input[i+1] << "\n";
        if(input[i] > n || input[i+1] > n) continue;
        if(!check_edge(input[i], input[i+1])) insert_edge(input[i], input[i+1]);
        else count++;

    }

    //fprintf(stderr, "elses %d / %d\n", count, m);
    
    delete[] max_indegree;
    delete[] max_outdegree;

}


int Graph_csr::getN() const {
    return n;
}

void Graph_csr::setN(int n) {
    Graph_csr::n = n;
}

int Graph_csr::getM() const {
    return m;
}

void Graph_csr::setM(int m) {
    Graph_csr::m = m;
}





int Graph_csr::is_out_adjacent_empty(int vertex){
    /** return true if vertex's out-adjacent list is empty, false otherwise
     * @param vertex is the vertex to check if it has out going edges
     * @return 1 if vertex has no out going edges, 0 otherwise
     */

    return o_First[vertex] < 0;
}

int Graph_csr::is_in_adjacent_empty(int vertex){
    /** return true if vertex's in-adjacent list is empty, false otherwise
     * @param vertex is the vertex to check if it has incoming edges
     * @return 1 if vertex has no incoming edges, 0 otherwise
     */

    return i_First[vertex] < 0;
}

void Graph_csr::insert_edge(int u,int v){
/** insert edge (u, v) */

    //store (u, v) as out edge
    o_Target[o_degree[u]++] = v;
    
    //store (u, v) as in edge
    i_Target[i_degree[v]++] = u;
}



int Graph_csr::delete_edge(int u,int v){
/** delete edge (u, v). Return 1 if (u,v) exists and is deleted */
    
    if(!this->delete_from_target(v, o_First[u], o_Target, o_degree[u])) return 0;
    if(this->delete_from_target(u, i_First[v], i_Target, i_degree[v])) return 0;
    o_degree[u]--;
    i_degree[v]--;
}


int Graph_csr::delete_from_target(int v, int first, int *Target, int end){
/* delete v from Target. first and end are the range of Target where looking for v
   Return 1 if v exists
*/
    int next_u = first;
    for(; next_u < end; next_u++){
        if(Target[next_u] == v) {
            break; //found edge (u, v)
        }
    }
    
    if(next_u == end) return 0; // if the first cycle does not end with the break then (u, v) does not exists
    
    for(; next_u < end - 1; next_u++){
        Target[next_u] = Target[next_u + 1];
    }
    
    return 1;
  

}


int Graph_csr::check_edge(int u,int v){
/** che if edge (u, v) already exists. Return the 1 if (u, v) is in G, 0 otherwise */

    for(int next_u = o_First[u]; next_u < o_degree[u]; next_u++){
        if(o_Target[next_u] == v) {
            return 1; //found edge (u, v)
        }
    }
    return 0;
}








void Graph_csr::print_graph(){
    int next, selected_node;
    int *buffer = this->o_First;

   /* std::cout<<std::endl;
    for(int i = 0; i < n + 2*m; i++){
        if(i == 0)std::cout<<"First\n";
        if(i == n)std::cout<<"Target\n";
        if(i == n+m)std::cout<<"Next\n";
        std::cout<<buffer[i]<<"\n";
    }*/


    for(int i = 1; i < n; i++) {

        for(next = o_First[i]; next < o_degree[i]; next++){
            selected_node = o_Target[next];
            std::cout << "Graph_csr::print_graph; edge " << i << "->" << selected_node << "    next = " << next << "\n";
        }
    }

    /*std::cout << "Graph_csr::print_graph; REVERSE EDGE\n";

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
            std::cout << "Graph_csr::print_graph; edge " << i << "->" << selected_node << "    u = " << u << "\n";
            u = i_Next[u];
        }
    }

    std::cout<<std::endl;*/
}





void Graph_csr::generate(std::string filename, int direction){
    std::cerr<<"Reading graph " << filename<<"\n";

    int nV, nE;
    int *graph = read_Graph(filename, &nV, &nE, direction);


    this->create(graph, nV, nE);

 
    //writeGraph_csr(graph, nV, nE, path + filename);
}




void Graph_csr::bfs_2(int s){




    int u, v;
    int head = 0, tail = 0;
    bfs_number++;
    

    
    visited[s] = bfs_number;
    //fprintf(stderr, "%d Visited vertex: %d\n", tid, s);
    
    //queue[tail++] = s;
    
    printf("%d ", s);
    
    for (int i = o_First[s]; i < o_degree[s]; i++) {
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
        for (int i = o_First[u]; i < o_degree[u]; i++) {
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


void Graph_csr::print_top_balls(int n_balls){

    fprintf(stderr, "sorting...\n");
    std::sort(degree, degree + n, comparePair);
    fprintf(stderr, "done\n");
    for (int i = 0; i < n_balls; i++){
        fprintf(stderr, "i = %d, n = %d -> n-1-i = %d\n", i, n, n-1-i);
        bfs_2(degree[n-1-i].first);
    }

}



int Graph_csr::bfs_gamma(int s, int *queue, char *visited, int *visited_vertices, int *indegrees, int *outdegrees, int *level){
// return the number of visited vertices, i.e., size of visited_vertices


    int u, v;
    int head = 0, tail = 0;
    int n_visited = 0;
    int max = 0;

    

    
    visited[s/32]= (1 << (s % 32));
    //fprintf(stderr, "%d Visited vertex: %d\n", tid, s);
    
    //queue[tail++] = s;
    
    //printf("s = %d \n", s);
    
    for (int i = o_First[s]; i < o_degree[s]; i++) {
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
        for (int i = o_First[u]; i < o_degree[u]; i++) {
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


void Graph_csr::compute_gamma(int start, int end = -1){

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
  
  
  
    fprintf(stderr, "Graph_csr's gamma is %d - vertices above the fixed threshold are %d (size n=%d, m=%d)\n", max_gamma, over_threshold, n, m);

}








