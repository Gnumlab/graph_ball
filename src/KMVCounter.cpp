#include <ostream>
#include <iostream>
#include <algorithm>
#include <limits>
#include "include/KMVCounter.h"

template class KMVCounter<uint32_t>;
template class KMVCounter<uint64_t>;

/* uint64_t implementation */
template <typename T>
KMVCounter<T>::KMVCounter(uint16_t k, TabulationHash<T> *h)
{
    this->k = k;
    this->values = new T[k];
    for (uint16_t i = 0; i < k; i++)
    {
        this->values[i] = std::numeric_limits<T>::max();
    }
    this->h = h;
    this->max_index = 0;
}

template <typename T>
KMVCounter<T>::~KMVCounter()
{
    delete[] this->values;
}

template <typename T>
void KMVCounter<T>::add(uint32_t x)
{
    T hash_value = (*this->h)(x);

    if (hash_value < this->values[this->max_index])
    {
        this->values[this->max_index] = hash_value;

        this->max_index = 0;
        for (uint16_t i = 0; i < this->k; i++)
        {
            if (this->values[i] > this->values[this->max_index])
            {
                this->max_index = i;
            }
        }
    }
}

template <typename T>
uint32_t KMVCounter<T>::size()
{
    return static_cast<uint32_t>(this->k * (double)std::numeric_limits<T>::max() / (double)this->values[this->max_index]);
}

template <typename T>
void KMVCounter<T>::merge(KMVCounter<T> *other)
{
    T temp[2 * this->k];

    for (uint16_t i = 0; i < this->k; i++)
    {
        temp[i] = this->values[i];
        temp[2 * i] = other->values[i];
    }

    std::sort(temp, temp + 2 * this->k);

    this->values[0] = temp[0];
    uint16_t i = 1, j = 1;
    while (i < this->k)
    {
        if (this->values[i - 1] != temp[j])
        {
            this->values[i] = temp[j];
            i++;
        }
        j++;
    }

    this->max_index = this->k - 1;
}