#ifndef LAZYBALL_H
#define LAZYBALL_H

#include "Ball.h"
#include <unordered_set>

class LazyBall : public Ball
{
private:
    /**
     * This is the set containing the vertices at distance 1.
     */
    std::unordered_set<uint32_t> ball1;

    /**
     * This is the set containing the vertices at distance 2.
     */
    std::unordered_set<uint32_t> ball2;
public:
    LazyBall();
    ~LazyBall();
    void insert(int v);
    void push(LazyBall *B);
    uint32_t size();
    void print();
    void flush(uint32_t x = UINT32_MAX);
};

#endif