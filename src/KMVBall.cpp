#include "include/KMVBall.h"

template class KMVBall<uint32_t>;
template class KMVBall<uint64_t>;

/**
 * This is the default constructor of the KMVBall class.
 */
template <typename T>
KMVBall<T>::KMVBall()
{
    this->ball1 = nullptr;
    this->ball2 = nullptr;
    this->hash = nullptr;
    this->k = 0;
}

/**
 * This is the parameterized constructor of the KMVBall class.
 * It initializes the ball1 and ball2 of the current KMVBall object as empty arrays of size k.
 * @param hash: array of hash functions used to hash the vertices.
 * @param k: number of hash functions used to hash the vertices.
 */
template <typename T>
KMVBall<T>::KMVBall(TabulationHash<T> *hash, uint16_t k)
{
    this->init(hash, k);
}

template <typename T>
void KMVBall<T>::init(TabulationHash<T> *hash, uint16_t k, uint32_t x)
{
    this->ball1 = new KMVCounter<T>(k, hash);
    this->ball2 = new KMVCounter<T>(k, hash);

    if (x != UINT32_MAX)
    {
        this->ball1->add(x);
        this->ball2->add(x);
    }

    this->hash = hash;
    this->k = k;
}

template <typename T>
KMVBall<T>::~KMVBall()
{
    if (this->ball1 != nullptr)
    {
        delete this->ball1;
        delete this->ball2;
    }
}

/**
 * This method is used to insert the given element v into the ball1 of the current KMVBall object.
 * @param v: element to be inserted into the ball1 =and ball2= of the current KMVBall object.
 * @return: void
 */
template <typename T>
void KMVBall<T>::insert(uint32_t v)
{
    this->ball1->add(v);
    // TODO: check this after updating Graph_csr<KMVBall> constructor
    // this->ball2->add(v);
}

/**
 * This method is used to push all the elements of the ball1 of the given KMVBall object B to the ball2 of the current KMVBall object.
 * @param B: KMVBall object whose ball1 elements are to be pushed to the ball2 of the current KMVBall object.
 * @return: void
 */
template <typename T>
void KMVBall<T>::push(KMVBall<T> *B)
{
    this->ball2->merge(B->ball1);
}

/**
 * This method estimates the size of the ball2 of the current KMVBall object using Flajolet-Martin estimation.
 * @return: estimated size of the ball2 of the current KMVBall object.
 */
template <typename T>
uint32_t KMVBall<T>::size()
{
    return this->ball2->size();
}

template <typename T>
void KMVBall<T>::flush(uint32_t x)
{
    this->ball1->flush();
    this->ball2->flush();

    this->ball1->add(x);
    this->ball2->add(x);
}

