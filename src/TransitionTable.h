#ifndef TRANSITIONTABLE_H
#define TRANSITIONTABLE_H

#include <map>
#include <string>

#include "matrix.h"

class Input;

class TransitionTable
{
public:
    TransitionTable(){ _numStates = 0; }

    void addInput( Input *input );

    virtual bool areOutputsEqual( int s1, int s2, size_t *retvalActionIdx = 0 ) = 0;
    void clearAlphabet( void );

    Input *getInputById( char id );
    Matrix<int> *getNextStateMatrixPtr(){ return &_nextStateMat; }
    Matrix<int> &getNextStateMatrixRef(){ return _nextStateMat; }
    size_t getNumberOfStates(){ return _numStates; }

    void loadInputsFromFile( std::string file );
    virtual void loadOutputsFromFile( std::string file ) = 0;
    void loadNextStatesFromFile( std::string file ){ _nextStateMat.loadFromFile( file ); }

    virtual void print( void ) = 0;
    virtual void resizeMatrices( void ) = 0;

    void setNumberOfStates( size_t numStates ){ _numStates = numStates; }

    virtual ~TransitionTable();
protected:
    size_t _numStates;
    std::map<char,Input*> _alphabet;
    Matrix<int> _nextStateMat;
};

#endif // TRANSITIONTABLE_H
