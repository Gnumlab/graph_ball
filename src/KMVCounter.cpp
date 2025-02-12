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
    if (this->h == nullptr)
    {
        std::cerr << "Hash function is null" << std::endl;
        exit(1);
    }
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

    if (hash_value < this->values[this->k - 1])
    {
        uint16_t i = 0;
        while (this->values[i] < hash_value)
            i++;

        T current = hash_value;
        T temp;

        for (; i < this->k; i++)
        {
            temp = this->values[i];
            this->values[i] = current;
            current = temp;
        }
    }
}

template <typename T>
uint32_t KMVCounter<T>::size()
{
    return static_cast<uint32_t>(this->k * (double)std::numeric_limits<T>::max() / (double)this->values[this->k - 1]);
}

template <typename T>
void KMVCounter<T>::merge(KMVCounter<T> *other)
{
    T temp[this->k];

    uint16_t i = 0, j = 0, k = 0;

    while (i < this->k && j < other->k && k < this->k)
    {
        if (this->values[i] < other->values[j])
        {
            temp[k++] = this->values[i++];
        }
        else
        {
            temp[k++] = other->values[j++];
        }
    }

    for (uint16_t i = 0; i < this->k; i++)
    {
        this->values[i] = temp[i];
    }
}

template <typename T>
void KMVCounter<T>::flush()
{
    for (uint16_t i = 0; i < this->k; i++)
    {
        this->values[i] = std::numeric_limits<T>::max();
    }
}