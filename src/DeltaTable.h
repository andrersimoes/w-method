#ifndef DELTATABLE_H
#define DELTATABLE_H

#include <set>
#include <string>

#include "matrix.h"

template<typename OutType>
class DeltaTable
{
public:
    DeltaTable(){}

    void addAction( std::string action ){ _alfabet.insert( action ); }
    size_t getNumberOfStates(){ return _numStates; }

    Matrix<int> *getNextStateMatrixPtr(){ return &_nextStateMat; }
    Matrix<OutType> *getOutMatrixPtr(){ return &_outputMat; }

    Matrix<int> &getNextStateMatrixRef(){ return _nextStateMat; }
    Matrix<OutType> &getOutMatrixRef(){ return _outputMat; }

    void print( void );
    void resizeMatrices( void );
    void setNumberOfStates( size_t numStates ){ _numStates = numStates; }

private:
    size_t _numStates;
    std::set<std::string> _alfabet;
    Matrix<OutType> _outputMat;
    Matrix<int> _nextStateMat;
};

#include "DeltaTable.hpp"

#endif // DELTATABLE_H
