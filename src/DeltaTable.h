#ifndef DELTATABLE_H
#define DELTATABLE_H

#include <set>
#include <string>

#include "matrix.h"

class DeltaTable
{
public:
    DeltaTable(){}

    void addAction( std::string action ){ _alfabet.insert( action ); }

    Matrix<int> *getNextStateMatrixPtr(){ return &_nextStateMat; }
    Matrix<int> *getOutMatrixPtr(){ return &_outputMat; }

    Matrix<int> &getNextStateMatrixRef(){ return _nextStateMat; }
    Matrix<int> &getOutMatrixRef(){ return _outputMat; }

    void print( void );
    void resizeMatrices( void );
    void setNumberOfStates( size_t numStates ){ _numStates = numStates; }

private:
    size_t _numStates;
    std::set<std::string> _alfabet;
    Matrix<int> _outputMat;
    Matrix<int> _nextStateMat;
};

#endif // DELTATABLE_H
