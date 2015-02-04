#ifndef TESTTREE_H
#define TESTTREE_h

#include <list>
#include <string>
#include <vector>

#include "matrix.h"

class DeltaTable;

class TestTree
{
public:
    TestTree();

    // generate and returns a P set for the machine
    std::list<std::string> processDeltaTable( DeltaTable *dt );

private:
    void buildTestTree( int state,
            std::vector<bool> *ptrDoneV,
            std::string *ptrBuffer );

    Matrix<int> *ptrOutMat;
    Matrix<int> *ptrNextMat;
    DeltaTable *deltaTable;

    std::list<std::string> testTreeList;
};

#endif // TESTTREE_H
