#include <ostream>
#include <iostream>
#include <algorithm>
#include <limits>
#include "include/KMVCounter.h"

template class KMVCounter<uint32_t>;
template class KMVCounter<uint64_t>;

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
    // if (this->h == nullptr)
    // {
    //     std::cerr << "Hash function is null" << std::endl;
    //     exit(1);
    // }
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

        if (this->values[i] == hash_value)
            return; // skip duplicate

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
            // Check if the value is unique
            if (k == 0 || temp[k - 1] != this->values[i])
            {
                temp[k++] = this->values[i++];
            }
            else
            {
                i++;
            }
        }
        else if (this->values[i] > other->values[j])
        {
            // Check if the value is unique
            if (k == 0 || temp[k - 1] != other->values[j])
            {
                temp[k++] = other->values[j++];
            }
            else
            {
                j++;
            }
        }
        else
        {
            // Check if the value is unique
            if (k == 0 || temp[k - 1] != this->values[i])
            {
                temp[k++] = this->values[i];
                i++;
                j++;
            }
            else
            {
                i++;
                j++;
            }
        }
    }

    for (uint16_t idx = 0; idx < k; idx++)
    {
        this->values[idx] = temp[idx];
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
