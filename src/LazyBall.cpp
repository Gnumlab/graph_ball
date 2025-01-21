#include "include/LazyBall.h"

/**
 * This is the default constructor of the LazyBall class.
 * It initializes the ball1 and ball2 of the current LazyBall object as empty unordered sets.
 */
LazyBall::LazyBall()
{
    this->ball1 = std::unordered_set<int>();
    this->ball2 = std::unordered_set<int>();
}

/**
 * This is the default destructor of the LazyBall class.
 */
LazyBall::~LazyBall() {}

/**
 * This method is used to insert the given element v into both ball1 and ball2 of the current LazyBall object.
 * @param v: element to be inserted into the ball1 and ball2 of the current LazyBall object.
 * @return: void
 */
void LazyBall::insert(int v)
{
    this->ball1.insert(v);
    this->ball2.insert(v);
}

/**
 * This method is used to push all the elements of the ball1 of the given LazyBall object B to the ball2 of the current LazyBall object.
 * @param B: LazyBall object whose ball1 elements are to be pushed to the ball2 of the current LazyBall object.
 * @return: void
 */
void LazyBall::push(LazyBall *B)
{
    for (int v : B->ball1)
        this->ball2.insert(v);
        // this->insert(v);
}

/**
 * This method is used to return the size of the ball2 of the current LazyBall object.
 * It returns the number of elements in the ball2 only since ball1 is always a subset of ball2.
 * @return: size of the ball2 of the current LazyBall object.
 */
int LazyBall::size()
{
    return this->ball2.size();
}

void LazyBall::print()
{
    std::cout << "\tB1: ";
    for (int v : this->ball1)
        std::cout << v << " ";
    std::cout << std::endl;

    std::cout << "\tB2: ";
    for (int v : this->ball2)
        std::cout << v << " ";
    std::cout << std::endl;
}