#ifndef DYNAMICTRANSITIONTABLE_H
#define DYNAMICTRANSITIONTABLE_H

#include "TransitionTable.h"

template<typename OutType>
class DynamicTransitionTable : public TransitionTable
{
public:
    DynamicTransitionTable(){}

    bool areOutputsEqual( int s1, int s2, size_t *retvalActionIdx = 0 );

    Matrix<OutType> *getOutMatrixPtr(){ return &_outputMat; }
    Matrix<OutType> &getOutMatrixRef(){ return _outputMat; }

    void print( void );
    void resizeMatrices( void );
private:
    Matrix<OutType> _outputMat;
};

#include "DynamicTransitionTable.hpp"

#endif /// DYNAMICTRANSITIONTABLE_H


