#include "MachineTest.h"
#include "TransitionTable.h"

MachineTest::MachineTest()
{
    clear();
}

void MachineTest::clear( void )
{
    minimal = false;
    numStates = 0;
    numRefinementClasses = 0;
    strActionL.clear();
    wL.clear();
}

void MachineTest::configure( TransitionTable *tt )
{
    std::list<std::string>::iterator it = strActionL.begin();
    while( it != strActionL.end() )
        tt->addAction( *it++ );

    tt->setNumberOfStates( numStates );
    tt->resizeMatrices();
}

void MachineTest::load( std::string filename )
{
    clear();
    std::string tmp;
    size_t quantity;

    std::ifstream file( filename.c_str() );

    if( ! file.is_open() )
    {
        std::cout << "MachineTest::load() - can't load file " << filename << std::endl;
        throw;
    }

    file >> tmp; // numberOfStates
    file >> numStates; 

    file >> tmp; // actions
    file >> quantity;

    while( quantity-- )
    {
        file >> tmp;
        strActionL.push_back( tmp );
    }

    file >> tmp; // minimal
    file >> tmp;
    if( tmp == "true" )
        minimal = true;
    else
        minimal = false;

    file >> tmp; // wset
    file >> quantity;

    while( quantity-- )
    {
        file >> tmp;
        wL.push_back( tmp );
    }

    file >> tmp; // RefinedPk (last PkTable)
    file >> quantity; // 

    numRefinementClasses = quantity;
}

void MachineTest::print()
{
    std::cout << "number of states " << numStates << std::endl;
    std::cout << "minimal " << minimal << std::endl;

    std::list<std::string>::iterator it = strActionL.begin();
    std::cout << "actions = ";
    while( it != strActionL.end() ) std::cout << *it++ << " ";
    std::cout << std::endl;

    it = wL.begin();
    std::cout << "wset = ";
    while( it != wL.end() ) std::cout << *it++ << " ";
    std::cout << std::endl;

    std::cout << "Number of refinement classes: " << numRefinementClasses << std::endl;
}
