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

    Refiner refiner;
    refiner.processTransitionTable( &table );
    std::list<std::string> diagL; 

    std::cout << std::endl;
    std::cout << "Minimal: " << ( ( refiner.isMachineMinimal() == true ) ? "true" : "false" ) << std::endl;

    if( refiner.isMachineMinimal() )
    {
        diagL = refiner.buildDiagSet( false );

        while( diagL.empty() == false ) 
        {
            std::cout << diagL.front() << std::endl;
            diagL.pop_front();
        }
    }

    return 0;
}
