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

        if (this->values[i] == hash_value) break;  // skip duplicate
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
void KMVCounter<T>::add_old(uint32_t x)
{
    T hash_value = (*this->h)(x);

    if (hash_value < this->values[this->max_index])
    {
        this->values[this->max_index] = hash_value;

        this->max_index = 0;
        for (uint16_t i = 1; i < this->k; i++)
        {
            if (this->values[i] > this->values[this->max_index])
            {
                this->max_index = i;
            }
        }
    }
    

}


template <typename T>
uint32_t KMVCounter<T>::size_old()
{
    return static_cast<uint32_t>(this->k * (double)std::numeric_limits<T>::max() / (double)this->values[this->max_index]);
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

    while (i < this->k && j < other->k && k < this->k) {
        if (this->values[i] < other->values[j]) {
            // Check if the value is unique
            if (k == 0 || temp[k - 1] != this->values[i]) {   // First, value (k = =) ia always unique
                temp[k++] = this->values[i++];
            } else {
                i++; // Skip duplicate
            }    
        } else if (this->values[i] > other->values[j]) {
            // Check if the value is unique
            if (k == 0 || temp[k - 1] != other->values[j]) {
                temp[k++] = other->values[j++];
            } else {
                j++; // Skip duplicate
            }
        } else {
            
            if (k == 0 || temp[k - 1] != this->values[i]) {// Values are equal, add one and skip both
                temp[k++] = this->values[i];
                i++;
                j++;
            } else {
                i++;
                j++;
            }
        }
    }

    // Copy unique values back to this->values
    for (uint16_t idx = 0; idx < k; idx++) {
        this->values[idx] = temp[idx];
}

}


template <typename T>
void KMVCounter<T>::merge_old(KMVCounter<T> *other)
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


template <typename T>
void KMVCounter<T>::flush()
{
    for (uint16_t i = 0; i < this->k; i++)
    {
        this->values[i] = std::numeric_limits<T>::max();
    }
}
