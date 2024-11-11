//
// Created by balmung on 11/03/19.
//

#include <cstdio>
#include <cstdlib>

#include <string>
#include <iostream>
#include <random>
#include <fstream>


#define MAXLINE       100   /* max length of input line */

/* read graph from input file */
int* read_Graph(const char *file, int *n, int *m) {


    FILE *input = fopen((char*)file, "r");
    if (!input) {
        fprintf(stdout, "Error opening file \"%s\".\n", file);
        exit(1);
    }

    int *input_file;

    char line[MAXLINE]; /* stores input line */

    int x, y, skip;
    int current_input_pos= 0;


    fgets(line, MAXLINE, input);
    if(sscanf(line, "%d %d", n, m) != 2){
        fprintf(stdout, "Error reading graph size (%s).\n", file);
        exit(1);
    }

    fprintf(stdout, "n = %d m = %d.\n", *n, *m);


    input_file = new int[2 * (*m)];

    while (fgets(line, MAXLINE, input) != NULL) {
        switch (line[0]) {
            case '\n':;
            case '\0':
                break;
            default:
                if (sscanf(line, "%d %d ", &x, &y) != 2) {
                    fprintf(stdout, "Error reading graph arc (%s).\n", file);
                    exit(1);
                }
//                fprintf(stdout, "%d->%d.\n", x, y);

                input_file[current_input_pos++] = x;
                input_file[current_input_pos++] = y;

        }
    }

    fprintf(stdout, "end reading %s.\n", file);

    fclose(input);

    return input_file;
}


/* read graph from input file */
int* read_Graph(std::string filename, int *n, int *m, int direction) {

    std::cerr<<"File " + filename + " reading started\n";
    int *input_file;

    int x, y, skip;
    int current_input_pos= 0;

    int max = 0;
    int from, to, avoid;
    int edgeCount = 0;


    std::ifstream file(filename);

    file >> *n;
    file >> *m;
    input_file = new int[2 * 2 * (*m)];
    *m = *m * (2 - direction); //if direction is 0 then graph is undirected and we save an edge twice
    while (file >> from)
    {
//        std::getline(file, line);
//        std::cout << "   line " << line <<std::endl;

        if(from > max) max = from;
        file >> to;
        if(to > max) max = to;
        edgeCount++;
//        file >> skip;
        from++; to++; //vertices begin from 1 not 0
        input_file[current_input_pos++] = from;
        input_file[current_input_pos++] = to;
        if(direction == 0){ // graph is undirected then must insert both edges (u, v) and (v, u)
            input_file[current_input_pos++] = to;
            input_file[current_input_pos++] = from;
        }
//        file >> avoid;
    }
    file.clear();

    file.close();
//    std::cout << "   close " << max << " " << edgeCount<<std::endl;
    std::cerr<<"File " + filename + " read, ";
    fprintf(stderr, "n = %d m = %d.\n", *n, *m);


    return input_file;
}


int *ErdosReniyGraph(int size, double prob, int seed, int *number_edges) {

    std::default_random_engine generator(seed);
    std::uniform_real_distribution<double> distribution(0, 1);
//    std::cout<<"ErdosReniyGraph::Graph bef new: " << 2 * int (size * size * prob + size)  << " \n";


    int count = 0;
    *number_edges = 0;
    clock_t start = clock();

    std::cout<<"ErdosReniyGraph::Graph created: " << 2 * int (size * size * prob + size)  << " \n";

    for(int i = 1; i < size+1; i++){
        for(int j = 1; j < size+1; j++) {
//            if(i == j-1 || (i - j)==size-1) continue;
            if( i == j ) continue;
            double p = distribution(generator);
//            std::cout<<"number of edges " << p << " \n";

            if(prob >= p) {
//                std::cout<<"ErdosReniyGraph::Graph created: " << i <<" -> "<< j << " " << count << " =? " << int (size * size * prob) + size << " \n";

                count++;

            }
        }
    }
    printf("Graph time %f\n", ( clock() - start ) / (double) CLOCKS_PER_SEC);

//    std::cout<<"Graph created: "<< count  << " \n";

    std::default_random_engine generator1(seed);
    int *buffer = new int[2 * int (count+1)];       //buffer to store vertices and edges, both in and out
    count = 0;

    for(int i = 1; i < size+1; i++){
        for(int j = 1; j < size+1; j++) {
//            if(i == j-1 || (i - j)==size-1) continue;
            if( i == j ) continue;
            double p = distribution(generator1);
//            std::cout<<"number of edges " << p << " \n";


            if(prob >= p) {
//                std::cout<<"ErdosReniyGraph::Graph created: " << i <<" -> "<< j << " " << count << " =? " << int (size * size * prob) + size << " \n";
                buffer[2 * count] = i;
                buffer[2 * count + 1] = j;
                count++;
                (*number_edges)++;

            }
        }
    }
//    *number_edges = count;
    std::cout<<"" << size <<", "<< count << ", "<<prob;

    return buffer;
}


void writeGraph(int *g, int n, int m, std::string filename){

    std::fstream file(filename, std::ios_base::out);

    int count = 0;
    file << std::to_string(n) + " " + std::to_string(m) + "\n";

    for (int i = 0; i < 2*m; i+=2){
        std::string from = std::to_string(g[i]);
        std::string to = std::to_string(g[i + 1]);
        file << from + " " + to + "\n";
        count++;

    }



    file.close();

}



// Comparator function for sorting based on value
bool comparePair(const std::pair<int, int> a, const std::pair<int, int> b) {
    return a.second < b.second;
}


