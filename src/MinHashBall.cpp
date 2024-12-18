#include "include/MinHashBall.h"

MinHashBall::MinHashBall()
{
    this->ball1 = nullptr;
    this->ball2 = nullptr;
    this->hash = nullptr;
    this->k = 0;
}

MinHashBall::MinHashBall(Hash<int> **hash, int k)
{
    this->ball1 = new int[k];
    this->ball2 = new int[k];

    for (int i = 0; i < k; i++)
    {
        this->ball1[i] = INT32_MAX;
        this->ball2[i] = INT32_MAX;
    }

    this->hash = hash;
    this->k = k;
}

void MinHashBall::insert(int v)
{
    for (int i = 0; i < this->k; i++)
    {
        int h = (*this->hash[i])(v);
        if (h < this->ball1[i])
            this->ball1[i] = h;

        if (h < this->ball2[i])
            this->ball2[i] = h;
    }
}

void MinHashBall::push(MinHashBall *B)
{
    for (int i = 0; i < this->k; i++)
    {
        int v = B->ball1[i];
        if (v < this->ball2[i])
            this->ball2[i] = v;
    }
}

int MinHashBall::size()
{
    // TO BE IMPLEMENTED
    return 0;
}

float MinHashBall::similarity(MinHashBall *B1, MinHashBall *B2)
{
    int count = 0;
    for (int i = 0; i < B1->k; i++)
    {
        if (B1->ball2[i] == B2->ball2[i])
            count++;
    }
    return (float)count / B1->k;
}