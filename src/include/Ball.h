#ifndef BALL_H
#define BALL_H

#include <iostream>
class Ball
{
public:
    virtual void insert(int) {}
    virtual void push(Ball *) {}
    virtual int size() { return 0; }
    static float similarity(Ball *, Ball *) { return 0.0; }
    virtual void print() {}
    virtual void flush(uint32_t) {}
};

#endif