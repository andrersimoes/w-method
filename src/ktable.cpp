#include "ktable.h"

#include "DeltaTable.h"

KTable::KTable()
{
    ptrOutM = ptrNextM = 0;
}

bool KTable::areNextStatesEqual( int s1, int s2 )
{
    bool equal = true;
    for( size_t j = 0; j < classIdxM.numCols(); ++j )
    {
        if( classIdxM[ s1 ][ j ] != classIdxM[ s2 ][ j ] )
        {
            equal = false;
            break;
        }
    }
    return equal;
}

bool KTable::areOutputsEqual( int s1, int s2 )
{
    bool equal = true;
    size_t cols = ptrOutM->numCols();
    for( size_t j = 0; j < cols; ++j )
    {
        if( ptrOutM->operator[]( s1 )[ j ] != ptrOutM->operator[]( s2  )[ j ] )
        {
            equal = false;
            break;
        }
    }
    return equal;
}

void KTable::buildFirstPartition( DeltaTable *deltaTable )
{
    ptrNextM = deltaTable->getNextStateMatrixPtr();
    ptrOutM = deltaTable->getOutMatrixPtr();

    std::list<int> stateList;

    for( size_t sIdx = 0; sIdx < ptrOutM->numRows(); ++sIdx )
        stateList.push_back( (int)sIdx );

    while( stateList.empty() == false )
    {
        int baseState = stateList.front();
        stateList.pop_front();

        std::list<int> equivStates;
        equivStates.push_back( baseState );

        std::list<int>::iterator it = stateList.begin();
        while( it != stateList.end() )
        {
            if( areOutputsEqual( baseState, *it ) )
            {
                equivStates.push_back( *it );
                it = stateList.erase( it );
            }
            else
                ++it;
        }

        EquivClass *ec = new EquivClass;
        ec->equivStateL = equivStates;

        equivClassV.push_back( ec );
    }

    createTableIndexes();
}

void KTable::clear( void )
{
    for( size_t i = 0; i < equivClassV.size(); ++i )
        delete equivClassV.at( i );
    equivClassV.clear();
}

void KTable::createIndexesForClass( EquivClass *ec )
{
    std::list<int>::iterator it, end;
    it = ec->equivStateL.begin();
    end = ec->equivStateL.end();

    char startChar = 'a';

    while( it != end )
    {
        int sIdx = *it;

        for( size_t j = 0; j < ptrNextM->numCols(); ++j )
        {
            int classIdx = getClassIdxByState( ptrNextM->operator[]( sIdx )[ j ] );
            classIdxM[ sIdx ][ j ] = startChar + classIdx;
        }

        ++it;
    }
}

void KTable::createTableIndexes( void )
{
    classIdxM.setSize( ptrOutM->numRows(), ptrOutM->numCols() );

    for( size_t i = 0; i < equivClassV.size(); ++i )
        createIndexesForClass( equivClassV.at( i ) );
}

int KTable::getClassIdxByState( int sIdx )
{
    for( size_t i = 0; i < equivClassV.size(); ++i )
    {
        EquivClass *ec = equivClassV.at( i );
        std::list<int>::iterator it, end;
        it = ec->equivStateL.begin();
        end = ec->equivStateL.end();

        while( it != end )
        {
            if( *it == sIdx ) return (int)i;
            ++it;
        }
    }

    return -1;
}

void KTable::print( void )
{
    for( size_t i = 0; i < equivClassV.size(); ++i )
    {
        EquivClass *ec = equivClassV.at( i );
        std::cout << std::endl << "Equiv class " << i + 1 << std::endl;

        std::list<int>::iterator it = ec->equivStateL.begin();
        while( it != ec->equivStateL.end() )
        {
            std::cout << *it + 1 << ' ';
            for( size_t j = 0; j < ptrNextM->numCols(); ++j )
                std::cout << ptrNextM->operator[]( *it )[ j ] + 1 << classIdxM[ *it ][ j ] << ' ';
            std::cout << std::endl;

            ++it;
        }
    }
}

KTable* KTable::refinePartition()
{
    KTable* refined = new KTable;

    for( size_t i = 0; i < equivClassV.size(); ++i )
    {
        EquivClass *ec = equivClassV.at( i );
        std::list<int> inCheckList = ec->equivStateL;

        while( inCheckList.empty() == false )
        {
            int baseState = inCheckList.front();
            inCheckList.pop_front();

            std::list<int> equivList;
            equivList.push_back( baseState );

            std::list<int>::iterator it = inCheckList.begin();

            while( it != inCheckList.end() )
            {
                if( areNextStatesEqual( baseState, *it ) )
                {
                    equivList.push_back( *it );
                    it = inCheckList.erase( it );
                }
                else
                    ++it;
            }

            EquivClass *ec = new EquivClass;
            ec->equivStateL = equivList;

            refined->equivClassV.push_back( ec );
        }
    }
    
    if( refined->equivClassV.size() != equivClassV.size() )
    {
        refined->ptrOutM = ptrOutM;
        refined->ptrNextM = ptrNextM;

        refined->createTableIndexes();
    }
    else
    {
        delete refined;
        refined = 0;
    }

    return refined;
}


KTable::~KTable()
{
    clear();
}

