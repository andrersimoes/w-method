#include "TestTree.h"
#include "DeltaTable.h"

TestTree::TestTree()
{
    deltaTable = 0;
}

std::list<std::string> TestTree::processDeltaTable( DeltaTable *dt )
{
    deltaTable = dt;
    ptrNextMat = dt->getNextStateMatrixPtr();
    ptrOutMat = dt->getOutMatrixPtr();

    std::string buffer;
    std::vector<bool> visitedStateV;
    visitedStateV.resize( deltaTable->getNumberOfStates() );

    testTreeList.clear();
    buildTestTree( 0, &visitedStateV, &buffer );
    return testTreeList;
}

void TestTree::buildTestTree( int state, std::vector<bool> *ptrDoneV, std::string *ptrBuffer )
{
    ptrDoneV->operator[]( state ) = true;

    for( size_t j = 0; j < ptrNextMat->getCols(); ++j )
    {
        int nextState = ptrNextMat->operator[]( state )[ j ];

        (*ptrBuffer) += j + 'a';

        std::cout << "From = " << state+1 << " to = " << nextState+1 << " --> " << *ptrBuffer << std::endl;

        testTreeList.push_back( *ptrBuffer );
        
        if( ptrDoneV->operator[]( nextState ) == false )
            buildTestTree( nextState, ptrDoneV, ptrBuffer );

        ptrBuffer->erase( ptrBuffer->end() - 1 );

    }
}
