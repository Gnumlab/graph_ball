#ifndef MINHASHBALL_H
#define MINHASHBALL_H

#include "Ball.h"
#include <unordered_set>
#include "../Hash.cpp"

class MinHashBall : public Ball
{
private:
    uint32_t* ball1;
    uint32_t* ball2;
    Hash<uint32_t>** hash;
    int k;
public:
    MinHashBall();
    MinHashBall(Hash<uint32_t>** hash, int k);
    ~MinHashBall();
    void init(Hash<uint32_t>** hash, int k);
    void insert(int v);
    void push(MinHashBall *B);
    int size();
    static float similarity(MinHashBall *B1, MinHashBall *B2);
    uint32_t* getSignature();
    void print();
    void flush();
};

#endif