#ifndef MINHASHBALL_H
#define MINHASHBALL_H

#include "Ball.h"
#include <unordered_set>
#include "../Hash.cpp"

class MinHashBall : public Ball
{
private:
    int* ball1;
    int* ball2;
    Hash<int>** hash;
    int k;
public:
    MinHashBall();
    MinHashBall(Hash<int>** hash, int k);
    void insert(int v);
    void push(MinHashBall *B);
    int size();
    static float similarity(MinHashBall *B1, MinHashBall *B2);
};

#endif