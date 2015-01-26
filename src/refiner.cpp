#include "refiner.h"

#include "DeltaTable.h"
#include "ktable.h"

Refiner::Refiner(){}

void Refiner::clear()
{
    while( ktableL.empty() == false )
    {
        delete ktableL.front();
        ktableL.pop_front();
    }
}

void Refiner::processDeltaTable( DeltaTable *deltaTable )
{
    KTable *partition = new KTable;

    partition->buildFirstPartition( deltaTable );
    ktableL.push_back( partition );

    while( ( partition = partition->refinePartition() ) )
    {
        std::cout << "----------------------------------"  << std::endl;
        partition->print();
        ktableL.push_back( partition );
    }
}

Refiner::~Refiner()
{
    clear();
}

