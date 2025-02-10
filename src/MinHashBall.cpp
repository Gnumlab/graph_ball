#include "include/MinHashBall.h"

/**
 * This is the default constructor of the MinHashBall class.
 */
MinHashBall::MinHashBall()
{
    this->ball1 = nullptr;
    this->ball2 = nullptr;
    this->hash = nullptr;
    this->k = 0;
}

/**
 * This is the parameterized constructor of the MinHashBall class.
 * It initializes the ball1 and ball2 of the current MinHashBall object as empty arrays of size k.
 * @param hash: array of hash functions used to hash the vertices.
 * @param k: number of hash functions used to hash the vertices.
 */
MinHashBall::MinHashBall(Hash<uint32_t> **hash, int k)
{
    this->init(hash, k);
    // this->ball1 = new uint32_t[k];
    // this->ball2 = new uint32_t[k];

    // for (int i = 0; i < k; i++)
    // {
    //     this->ball1[i] = UINT32_MAX;
    //     this->ball2[i] = UINT32_MAX;
    // }

    // this->hash = hash;
    // this->k = k;
}

void MinHashBall::init(Hash<uint32_t> **hash, int k, uint32_t x)
{
    this->ball1 = new uint32_t[k];
    this->ball2 = new uint32_t[k];

    for (int i = 0; i < k; i++)
    {
        if (x == UINT32_MAX)
        {
            this->ball1[i] = UINT32_MAX;
            this->ball2[i] = UINT32_MAX;
        }
        else
        {
            this->ball1[i] = (*hash[i])(x);
            this->ball2[i] = (*hash[i])(x);
        }
    }

    this->hash = hash;
    this->k = k;
}

MinHashBall::~MinHashBall()
{
    if (this->ball1 != nullptr)
    {
        delete[] this->ball1;
        delete[] this->ball2;
    }
}

/**
 * This method is used to insert the given element v into the ball1 of the current MinHashBall object.
 * @param v: element to be inserted into the ball1 =and ball2= of the current MinHashBall object.
 * @return: void
 */
void MinHashBall::insert(int v)
{
    for (int i = 0; i < this->k; i++)
    {
        uint32_t h = (*this->hash[i])(v);
        if (h < this->ball1[i])
            this->ball1[i] = h;

        // TODO: check this after updating Graph_csr<MinHashBall> constructor
        // if (h < this->ball2[i])
        //     this->ball2[i] = h;
    }
}

/**
 * This method is used to push all the elements of the ball1 of the given MinHashBall object B to the ball2 of the current MinHashBall object.
 * @param B: MinHashBall object whose ball1 elements are to be pushed to the ball2 of the current MinHashBall object.
 * @return: void
 */
void MinHashBall::push(MinHashBall *B)
{
    for (int i = 0; i < this->k; i++)
    {
        uint32_t v = B->ball1[i];
        if (v < this->ball2[i])
            this->ball2[i] = v;
    }
}

/**
 * This method estimates the size of the ball2 of the current MinHashBall object using Flajolet-Martin estimation.
 * @return: estimated size of the ball2 of the current MinHashBall object.
 */
int MinHashBall::size()
{
    float size = 0.0;
    for (int i = 0; i < this->k; i++)
        size += (float)UINT32_MAX / (float)this->ball2[i] - 1;
    return static_cast<int>(size / (float)this->k);
}

/**
 * This method estimates the similarity between the ball2 of the given MinHashBall objects B1 and B2 using minhash similarity estimation.
 * @param B1: first MinHashBall object.
 * @param B2: second MinHashBall object.
 * @return: estimated similarity between the ball2 of the given MinHashBall objects B1 and B2.
 */
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

/**
 * This method returns the minhash signature of the ball2 of the current MinHashBall object.
 * @return: signature of the current MinHashBall object.
 */
uint32_t *MinHashBall::getSignature()
{
    return this->ball2;
}

void MinHashBall::print()
{
    std::cout << "\tS(B1): ";
    for (int i = 0; i < this->k; i++)
    {
        std::cout << this->ball1[i] << " ";
        if (i == 10)
        {
            std::cout << "...";
            break;
        }
    }
    std::cout << std::endl;

    std::cout << "\tS(B2): ";
    for (int i = 0; i < this->k; i++)
    {
        std::cout << this->ball2[i] << " ";
        if (i == 10)
        {
            std::cout << "...";
            break;
        }
    }
    std::cout << std::endl;
}

void MinHashBall::flush(uint32_t x)
{
    for (int i = 0; i < this->k; i++)
    {
        if (x == UINT32_MAX)
        {
            this->ball1[i] = UINT32_MAX;
            this->ball2[i] = UINT32_MAX;
        }
        else
        {
            this->ball1[i] = (*this->hash[i])(x);
            this->ball2[i] = (*this->hash[i])(x);
        }
    }
}