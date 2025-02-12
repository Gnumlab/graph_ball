#ifndef MINHASHBALL_H
#define MINHASHBALL_H

#include "Ball.h"
#include <unordered_set>
#include "../Hash.cpp"

class MinHashBall : public Ball
{
public:
    uint32_t *ball1;
    uint32_t *ball2;
    Hash<uint32_t> **hash;
    int k;

public:
    MinHashBall();
    MinHashBall(Hash<uint32_t> **hash, int k);
    ~MinHashBall();
    void init(Hash<uint32_t> **hash, int k, uint32_t x = UINT32_MAX);
    void insert(int v);
    void push(MinHashBall *B);
    uint32_t size();
    static float similarity(MinHashBall *B1, MinHashBall *B2);
    uint32_t *getSignature();
    void print();
    void flush(uint32_t x = UINT32_MAX);
};

#endif
