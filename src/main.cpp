#include <iostream>
/*#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include "matrix.h"
*/

#include "DeltaTable.h"
#include "refiner.h"

int main()
{
    DeltaTable table;

    table.addAction( "alfa" );
    table.addAction( "beta" );
    //table.addAction( "gamma" );
    //table.setNumberOfStates( 9 );
    table.setNumberOfStates( 5 );

    table.resizeMatrices();

    Matrix<int> &outM = table.getOutMatrixRef();
    Matrix<int> &stateM = table.getNextStateMatrixRef();

    outM.loadFromFile( "../data/outTable-A17.txt", true ); 
    stateM.loadFromFile( "../data/nextTable-A17.txt", true ); stateM += -1;

    Refiner refiner;
    refiner.processDeltaTable( &table );

    std::cout << "Minimal: " << ( ( refiner.isMachineMinimal() == true ) ? "true" : "false" ) << std::endl;

    refiner.buildDiagSet();
    //table.print();

    return 0;
}
