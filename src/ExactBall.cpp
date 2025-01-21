#include "include/ExactBall.h"

ExactBall::ExactBall()
{
    this->ball = std::unordered_set<int>();
    this->last_element = -1;
}

ExactBall::~ExactBall() {}

void ExactBall::insert(int v)
{
    this->ball.insert(v);
    this->last_element = v;
}

void ExactBall::push(ExactBall *B)
{
    this->ball.insert(B->last_element);
}

int ExactBall::size()
{
    return this->ball.size();
}