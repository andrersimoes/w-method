#ifndef TRANSITIONTABLE_H
#define TRANSITIONTABLE_H

#include <set>
#include <string>

#include "matrix.h"

class TransitionTable
{
public:
    TransitionTable(){ _numStates = 0; }

    void addAction( std::string action ){ _alfabet.insert( action ); }

    virtual bool areOutputsEqual( int s1, int s2, size_t *retvalActionIdx = 0 ) = 0;

    Matrix<int> *getNextStateMatrixPtr(){ return &_nextStateMat; }
    Matrix<int> &getNextStateMatrixRef(){ return _nextStateMat; }
    size_t getNumberOfStates(){ return _numStates; }

    virtual void print( void ) = 0;
    virtual void resizeMatrices( void ) = 0;

    void setNumberOfStates( size_t numStates ){ _numStates = numStates; }
protected:
    size_t _numStates;
    std::set<std::string> _alfabet;
    Matrix<int> _nextStateMat;
};

#endif // TRANSITIONTABLE_H
