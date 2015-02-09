#include <iostream>
#include <cmath>
#include <cstdlib>

#include "refiner.h"
#include "DynamicTransitionTable.h"
#include "output.h"

int main()
{
    DynamicTransitionTable <Output> table;
    table.addAction( "a" );
    table.addAction( "b" );
    table.addAction( "c" );
    table.addAction( "d" );
    table.addAction( "e" );
    table.addAction( "f" );

    table.setNumberOfStates( 4 );
    table.resizeMatrices();

    Matrix<Output> &outM = table.getOutMatrixRef();
    Matrix<int> &nextM = table.getNextStateMatrixRef();

    nextM.loadFromFile( "../data/nextTable-sat01.txt" );
    outM.loadFromFile( "../data/outTable-sat01.txt" );

    nextM.print();
    outM.print();

    Refiner refiner;
    refiner.processTransitionTable( &table );
    std::list<std::string> diagL = refiner.buildDiagSet( false );

    std::cout << std::endl;
    std::cout << "Minimal: " << ( ( refiner.isMachineMinimal() == true ) ? "true" : "false" ) << std::endl;

    while( diagL.empty() == false ) 
    {
        std::cout << diagL.front() << std::endl;
        diagL.pop_front();
    }

    return 0;
}
