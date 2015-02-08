#include "TestTree.h"
#include "TransitionTable.h"

TestTree::TestTree()
{
    transitionTable = 0;
}

std::list<std::string> TestTree::processTransitionTable( TransitionTable *tt )
{
    transitionTable = tt;
    ptrNextMat = tt->getNextStateMatrixPtr();

    std::string buffer;
    std::vector<bool> visitedStateV;
    visitedStateV.resize( transitionTable->getNumberOfStates() );

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

void TestTree::print( void )
{
    std::list<std::string>::iterator it, end;
    it = testTreeList.begin();
    end = testTreeList.end();

    std::cout << "TestTree::print() - P set for last transition table" << std::endl;

    while( it != end )
        std::cout << *it++ << std::endl;
}
