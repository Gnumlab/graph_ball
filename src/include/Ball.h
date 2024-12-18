#ifndef BALL_H
#define BALL_H

class Ball
{
public:
    virtual void insert(int) {}
    virtual void push(Ball *) {}
    virtual int size() { return 0; }
    static float similarity(Ball *, Ball *) { return 0.0; }
};

#endif