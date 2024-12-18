#include "include/LazyBall.h"

LazyBall::LazyBall()
{
    this->ball1 = std::unordered_set<int>();
    this->ball2 = std::unordered_set<int>();
}

void LazyBall::insert(int v)
{
    this->ball1.insert(v);
    this->ball2.insert(v);
}

void LazyBall::push(LazyBall *B)
{
    // LazyBall *ball = dynamic_cast<LazyBall *>(B);
    for (int v : B->ball1)
        this->ball2.insert(v);
}

int LazyBall::size()
{
    return this->ball2.size();
}