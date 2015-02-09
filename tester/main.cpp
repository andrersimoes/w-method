#include <iostream>
#include <fstream>

#include "output.h"
#include "ktable.h"
#include "refiner.h"
#include "DynamicTransitionTable.h"
#include "MachineTest.h"


void execute( std::string testfile, std::string outType, std::string strNextFile, std::string strOutFile )
{
    TransitionTable *table;
    //DynamicTransitionTable<int> *table;

    if( outType == "int" )
        table = new DynamicTransitionTable<int>();
    else if( outType == "Output" )
        table = new DynamicTransitionTable<Output>();

    std::string path = "../data/";
    std::string testPath = "../data/testCases/";

    std::cout << std::endl;
    std::cout << "Starting test " << testfile << std::endl;
    MachineTest mt;
    mt.load( testPath + testfile );
    mt.configure( table );
    mt.print();

    table->loadNextStatesFromFile( path + strNextFile );
    table->getNextStateMatrixRef() += -1;
    table->loadOutputsFromFile( path + strOutFile );

    Refiner refiner;
    refiner.processTransitionTable( table );

    bool testPassed = true;

    if( mt.minimal )
    {
        if( refiner.isMachineMinimal() && mt.wL.empty() == false )
        {
            std::list<std::string> diagL = refiner.buildDiagSet( true );

            std::list<std::string>::iterator it1, it2;
            it1 = diagL.begin();
            it2 = mt.wL.begin();

            while( it1 != diagL.end() && it2 != mt.wL.end() )
            {
                if( *it1 != *it2 )
                {
                    std::cout << "w element expected: " << *it2 << std::endl;
                    std::cout << "w found: " << *it1 << std::endl;
                    testPassed = false;
                }
                ++it1; ++it2;
            }


            if( it1 != diagL.end() || it2 != mt.wL.end() )
            {
                std::cout << "wset expected size: " << mt.wL.size() << std::endl;
                std::cout << "wset size found: " << diagL.size() << std::endl;
                testPassed = false;
            }
        }
    }

    if( mt.minimal != refiner.isMachineMinimal() )
    {
        std::cout << "Expected minimality: " << mt.minimal << std::endl;
        std::cout << "Minimality found: " << refiner.isMachineMinimal() << std::endl;
        testPassed = false;    
    }

    const std::vector<KTable*> &ktableV = refiner.getKTableVectorRef();

    if( ktableV.empty() == false )
    {
        KTable *lastTable = ktableV.back();
        std::vector<EquivClass*> & classV = lastTable->getEquivClassVectorRef();

        if( classV.size() != mt.numRefinementClasses )
        {
            std::cout << "Expected number of refinement classes: " << mt.numRefinementClasses << std::endl;
            std::cout << "Number of refinement classes found: " << classV.size() << std::endl;
            testPassed = false;    
        }
    }

    std::cout << "Veredict: ";
    if( testPassed ) 
        std::cout << "PASSED";
    else 
        std::cout << "FAILED";
    std::cout << std::endl;

    delete table;
}

int main( void )
{
    execute( "test-A17.txt", "int", "nextTable-A17.txt", "outTable-A17.txt" );
    execute( "test-A7.txt", "int", "nextTable-A7.txt", "outTable-A7.txt" );
    execute( "test-sat01.txt", "Output", "nextTable-sat01.txt", "outTable-sat01.txt" );

    return 0;
}
