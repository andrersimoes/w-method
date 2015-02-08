#include "refiner.h"

#include "TransitionTable.h"
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
    if( transitionTable->getNumberOfStates() == 
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

    Matrix<int> *ptrNextStateM = transitionTable->getNextStateMatrixPtr();

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

    size_t actionIdx;
    transitionTable->areOutputsEqual( currS1, currS2, &actionIdx );
    strSequence += ( baseChar + (char)actionIdx );

    return strSequence;
}

std::list<std::string> Refiner::buildDiagSet( bool fullSet )
{
    std::list<std::string> retval;

    if( fullSet ) buildFullDiagSet( retval );
    else buildUniqueStringSet( retval );

    return retval;
}


void Refiner::buildFullDiagSet( std::list<std::string> &retval )
{
    int numStates = (int)  transitionTable->getNumberOfStates();
    for( int s1 = 0; s1 < numStates; ++s1 )
        for( int s2 = s1+1; s2 < numStates; ++s2 )
            retval.push_back( buildDiagSequence( s1, s2 ) );
}

void Refiner::buildUniqueStringSet( std::list<std::string> &retval )
{
    int numStates = (int)  transitionTable->getNumberOfStates();
    for( int s1 = 0; s1 < numStates; ++s1 )
    {
        for( int s2 = s1+1; s2 < numStates; ++s2 )
        {
            std::string sequence = buildDiagSequence( s1, s2 );
            keepGreaterElementOnList( sequence, retval );
        }
    }

    /*std::list<std::string>::iterator it, end;
    it = retval.begin(); end = retval.end();
    while( it != end ) std::cout << *it++ << std::endl;*/

}

void Refiner::keepGreaterElementOnList( std::string &strElement,
        std::list<std::string> &list )
{
    std::list<std::string>::iterator it, end;
    it = list.begin();
    end = list.end();

    size_t baseSize = strElement.size();

    while( it != end )
    {
        size_t secSize = it->size();
        size_t minSize = ( baseSize < secSize ) ? baseSize : secSize;
        bool equal = true;

        for( size_t i = 0; i < minSize; ++i )
        {
            if( strElement[ i ] != it->at( i ) )
            {
                equal = false;
                break;
            }
        }

        if( equal )
        {
            if( baseSize > secSize )
            {
               it = list.erase( it );
               list.push_back( strElement );
            }
            return;
        }

        ++it;
    }

    list.push_back( strElement );
}


void Refiner::processTransitionTable( TransitionTable *dt )
{
    transitionTable = dt;
    KTable *partition = new KTable;

    partition->buildFirstPartition( transitionTable );
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

