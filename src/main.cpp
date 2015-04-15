#include <iostream>
#include <cmath>
#include <cstdlib>

#include "wmethod.h"
#include "DynamicTransitionTable.h"
#include "output.h"
#include "TestWriter.h"

void printTestInputs( std::list<std::string> &testCaseList );

int main()
{
    std::string fileMiddleName = "sunEarthInsufficientRef";
    DynamicTransitionTable <Output> table;

    table.loadInputsFromFile( "../data/input-" + fileMiddleName + ".txt" );
    table.setNumberOfStates( 3 );
    table.resizeMatrices();

    Matrix<Output> &outM = table.getOutMatrixRef();
    Matrix<int> &nextM = table.getNextStateMatrixRef();

    nextM.loadFromFile( "../data/nextTable-" + fileMiddleName +".txt" );
    outM.loadFromFile( "../data/outTable-" + fileMiddleName +".txt" );

    nextM += -1;

    nextM.print();
    outM.print();

    std::list<std::string> testCaseList;
    WMethod wmethod;
    wmethod.processTransitionTable( &table, &testCaseList );
    //printTestInputs( testCaseList );

    TestWriter testWriter( &table, &testCaseList);
    testWriter.loadConstraints( "../data/constraints-" + fileMiddleName + ".txt" );
    testWriter.writeKeywordTest( std::cout );

    return 0;
}

void printTestInputs( std::list<std::string> &testCaseList )
{
    std::list<std::string>::iterator it = testCaseList.begin();
    while( it != testCaseList.end() )
    {
        std::cout << *it << std::endl;
        ++it;
    }
    std::cout << "number of test case input sequences " << testCaseList.size() << std::endl;
}
