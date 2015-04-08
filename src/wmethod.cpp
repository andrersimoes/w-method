#include "wmethod.h"
#include "refiner.h"
#include "TestTree.h"

WMethod::WMethod()
{
}

void WMethod::processTransitionTable( TransitionTable *table, std::list<std::string> *ptrTestCaseList, bool fullDiagSet )
{
    Refiner refiner;
    refiner.processTransitionTable( table );

    if( refiner.isMachineMinimal() )
    {
        std::list<std::string> diagL = refiner.buildDiagSet( fullDiagSet );  

        TestTree tree;
        std::list<std::string> treePathL = tree.processTransitionTable( table );

        std::list<std::string>::iterator pathIt, diagIt;
        diagIt = diagL.begin();

        while( diagIt != diagL.end() )
        {
            ptrTestCaseList->push_back( *diagIt );

            pathIt = treePathL.begin();
            while( pathIt != treePathL.end() )
            {
                ptrTestCaseList->push_back( *pathIt + *diagIt );
                ++pathIt;
            }
            ++diagIt;
        }
    }
}
