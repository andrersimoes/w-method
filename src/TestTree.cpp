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
    std::list< std::pair<int,std::string> > nodeList;
    std::vector<bool> visitedStateV;
    visitedStateV.resize( transitionTable->getNumberOfStates() );

    testTreeList.clear();

    visitedStateV[ 0 ] = true;
    nodeList.push_back( std::pair<int,std::string>( 0, "" ) );

    while( ! nodeList.empty() )
    {
        int nodeIdx = nodeList.front().first;
        std::string inputSeq = nodeList.front().second;
        if( ! inputSeq.empty() ) testTreeList.push_back( inputSeq );

        for( size_t j = 0; j < ptrNextMat->getCols(); ++j )
        {
            int nextState = ptrNextMat->operator[]( nodeIdx )[ j ];

            char input = 'a' + j;

            if( ! visitedStateV[ nextState ] )
            {
                visitedStateV[ nextState ] = true;
                nodeList.push_back( std::pair<int,std::string>( nextState, inputSeq + input ) );
            }
            else
                testTreeList.push_back( inputSeq + input );
        }

        nodeList.pop_front();
    }

    return testTreeList;
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
