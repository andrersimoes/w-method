#include <iostream>
#include <cmath>
#include <cstdlib>

#include "wmethod.h"
#include "DynamicTransitionTable.h"
#include "output.h"

int main()
{
    DynamicTransitionTable <Output> table;
    table.addAction( "a" );
    table.addAction( "b" );
    table.addAction( "c" );
    table.addAction( "d" );
    //table.addAction( "e" );
    //table.addAction( "f" );

    table.setNumberOfStates( 3 );
    table.resizeMatrices();

    Matrix<Output> &outM = table.getOutMatrixRef();
    Matrix<int> &nextM = table.getNextStateMatrixRef();

    std::string fileMiddleName = "sunEarthInsuficientRef";

    nextM.loadFromFile( "../data/nextTable-" + fileMiddleName +".txt" );
    outM.loadFromFile( "../data/outTable-" + fileMiddleName +".txt" );

    nextM += -1;

    nextM.print();
    outM.print();

    std::list<std::string> testCaseList;
    WMethod wmethod;
    wmethod.processTransitionTable( &table, &testCaseList );

    std::list<std::string>::iterator it = testCaseList.begin();
    while( it != testCaseList.end() )
    {
        std::cout << *it << std::endl;
        ++it;
    }
    std::cout << "number of test case input sequences " << testCaseList.size() << std::endl;

    return 0;
}
