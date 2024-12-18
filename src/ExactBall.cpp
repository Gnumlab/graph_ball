#include "include/ExactBall.h"

ExactBall::ExactBall()
{
    this->ball = std::unordered_set<int>();
    this->last_insert_element = -1;
}

void ExactBall::insert(int v)
{
    this->ball.insert(v);
    this->last_insert_element = v;
}

void ExactBall::push(ExactBall *B)
{
    // ExactBall *ball = dynamic_cast<ExactBall *>(B);
    this->ball.insert(B->last_insert_element);
    // for (int v : ball->ball)
    //     this->ball.insert(v);
}

int ExactBall::size()
{
    return this->ball.size();
}