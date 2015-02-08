#ifndef TESTTREE_H
#define TESTTREE_h

#include <list>
#include <string>
#include <vector>

#include "matrix.h"

class TransitionTable;

class TestTree
{
public:
    TestTree();

    // generate and returns a P set for the machine
    std::list<std::string> processTransitionTable( TransitionTable *tt );

    void print( void );

private:
    void buildTestTree( int state,
            std::vector<bool> *ptrDoneV,
            std::string *ptrBuffer );

    Matrix<int> *ptrNextMat;
    TransitionTable *transitionTable;

    std::list<std::string> testTreeList;
};

#endif // TESTTREE_H
