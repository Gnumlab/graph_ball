#ifndef LAZYBALL_H
#define LAZYBALL_H

#include "Ball.h"
#include <unordered_set>

class LazyBall : public Ball
{
private:
    std::unordered_set<int> ball1;
    std::unordered_set<int> ball2;
public:
    LazyBall();
    void insert(int v);
    void push(LazyBall *B);
    int size();
};

#endif