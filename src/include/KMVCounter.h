#ifndef KMVCOUNTER_H
#define KMVCOUNTER_H

#include "../Hash.cpp"
#include <limits>
#include <type_traits>

template <typename T>
class KMVCounter
{
    static_assert(std::is_same<T, uint32_t>::value || std::is_same<T, uint64_t>::value, "KMVCounter supports only uint32_t and uint64_t");

private:
    T *values;
    TabulationHash<T> *h;
    uint16_t k;
    uint16_t max_index;

public:
    KMVCounter(uint16_t, TabulationHash<T> *);
    ~KMVCounter();
    void add_old(uint32_t);
    uint32_t size_old();
    void add(uint32_t);
    uint32_t size();
    void merge(KMVCounter *);
    void merge_old(KMVCounter *);
    void flush();
};

#endif
