#include "refiner.h"

#include "DeltaTable.h"
#include "ktable.h"

Refiner::Refiner(){}

void Refiner::clear()
{
    for( size_t i = 0; i < ktableV.size(); ++i )
        delete ktableV.at( i );
    ktableV.clear();
}

bool Refiner::isMachineMinimal( void )
{
    if( ktableV.empty() ) return false;

    KTable *refinement = ktableV.back();
    if( deltaTable->getNumberOfStates() == 
            refinement->getEquivClassVectorRef().size() )
        return true;
    else
        return false;
}

std::string Refiner::buildDiagSequence( int s1, int s2 )
{
    std::string strSequence;
    size_t idxL;

    for( idxL = 0; idxL < ktableV.size(); ++idxL )
    {
        if( ktableV[ idxL ]->areStatesInTheSameClass( s1, s2 ) == false )
        {
            ++idxL;
            break;
        }
    }    

    size_t idxK = 1;
    int currS1 = s1;
    int currS2 = s2;
    char baseChar = 'a';

    Matrix<int> *ptrNextStateM = deltaTable->getNextStateMatrixPtr();

    while( idxL - idxK > 0 )
    {
        size_t mIdx = idxL - idxK;
        Matrix<char> &classIdxM = ktableV[ mIdx ]->getClassIdxMatrixRef();
        
        for( size_t j = 0; j < classIdxM.numCols(); ++j )
        {
            if( classIdxM[ currS1 ][ j ] != classIdxM[ currS2 ][ j ] )
            {
                strSequence += ( baseChar + (char)j );
                currS1 = ptrNextStateM->operator[]( currS1 )[ j ];
                currS2 = ptrNextStateM->operator[]( currS2 )[ j ];

                break;
            }
        }

        ++idxK;
    }

    Matrix<int> *ptrOutM = deltaTable->getOutMatrixPtr();

    for( size_t j = 0; j < ptrOutM->numCols(); ++j )
    {
        if( ptrOutM->operator[]( currS1 )[ j ] != 
            ptrOutM->operator[]( currS2 )[ j ] )
        {
            strSequence += ( baseChar + (char)j );
            break;
        }
    }

    return strSequence;
}

void Refiner::buildDiagSet( void )
{
    buildDiagSequence( 3, 4 );
}

void Refiner::processDeltaTable( DeltaTable *dt )
{
    deltaTable = dt;
    KTable *partition = new KTable;

    partition->buildFirstPartition( deltaTable );
    ktableV.push_back( partition );

    while( ( partition = partition->refinePartition() ) )
    {
        //std::cout << "----------------------------------"  << std::endl;
        //partition->print();
        ktableV.push_back( partition );
    }
}

Refiner::~Refiner()
{
    clear();
}

