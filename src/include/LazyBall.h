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
    std::unordered_set<int> ball1;

    /**
     * This is the set containing the vertices at distance 2.
     */
    std::unordered_set<int> ball2;
public:
    LazyBall();
    ~LazyBall();
    void insert(int v);
    void push(LazyBall *B);
    int size();
    void print();
    void flush();
};

#endif