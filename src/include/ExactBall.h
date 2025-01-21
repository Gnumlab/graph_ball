#ifndef EXACTBALL_H
#define EXACTBALL_H

#include "Ball.h"
#include <unordered_set>

class ExactBall : public Ball
{
private:
    std::unordered_set<int> ball;
    int last_element;
public:
    ExactBall();
    ~ExactBall();
    void insert(int v);
    void push(ExactBall *B);
    int size();
};

#endif