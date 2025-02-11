#ifndef KMVBALL_H
#define KMVBALL_H

#include "Ball.h"
#include "KMVCounter.h"
#include "../Hash.cpp"

template <typename T>
class KMVBall : public Ball<T>
{
public:
    KMVCounter<T> *ball1;
    KMVCounter<T> *ball2;
    TabulationHash<T> *hash;
    int k;

public:
    KMVBall();
    KMVBall(TabulationHash<T> *hash, int k);
    ~KMVBall();
    void init(TabulationHash<T> *hash, int k, uint32_t x = UINT32_MAX);
    void insert(uint32_t v);
    void push(KMVBall *B);
    uint32_t size();
    void flush(uint32_t x = UINT32_MAX);
};

#endif