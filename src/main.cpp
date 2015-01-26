#include <iostream>
#include <set>
#include <vector>
#include <list>
#include <algorithm>
#include "matrix.h"

class DeltaTable
{
public:
    DeltaTable(){}

    void addAction( std::string action ){ _alfabet.insert( action ); }

    Matrix<int> *getNextStateMatrixPtr(){ return &_nextStateMat; }
    Matrix<int> *getOutMatrixPtr(){ return &_outputMat; }

    Matrix<int> &getNextStateMatrixRef(){ return _nextStateMat; }
    Matrix<int> &getOutMatrixRef(){ return _outputMat; }

    void resizeMatrices()
    {
        _outputMat.setSize( _numStates, _alfabet.size() );
        _nextStateMat.setSize( _numStates, _alfabet.size() ); 
    }

    void print( void )
    {
        std::set<std::string>::iterator alfaStart, alfaIt, alfaEnd;
        alfaStart = _alfabet.begin();
        alfaEnd = _alfabet.end();

        std::cout << " state | ";
        for( unsigned short i = 0; i < 2; ++i )
        {
            alfaIt = alfaStart;
            while( alfaIt != alfaEnd )
                std::cout << *alfaIt++ << " | ";
        }
        std::cout << std::endl;

        for( size_t i = 0; i < _numStates; ++i )
        {
            std::cout << "   " << i << "   | ";

            for( size_t j = 0; j < _outputMat.numCols(); ++j ) 
                std::cout << "  " << _outputMat[ i ][ j ] << "  | ";

            std::cout << "|";

            for( size_t j = 0; j < _nextStateMat.numCols(); ++j ) 
                std::cout << "  " << _nextStateMat[ i ][ j ] << "  | ";

            std::cout << std::endl;
        }
    }

    void setNumberOfStates( size_t numStates ){ _numStates = numStates; }

private:
    size_t _numStates;
    std::set<std::string> _alfabet;
    Matrix<int> _outputMat;
    Matrix<int> _nextStateMat;
};


typedef std::list< std::pair<int,int> > IndexList; // <numero do estado, numero da linha correspondente em classIdxM>

struct EquivClass
{
    IndexList equivStateL;
    Matrix<char> classIdxM;
};

class KTable
{
public:
    KTable()
    {
        ptrOutM = ptrNextM = 0;
    }

    void buildFirstPartition( DeltaTable *deltaTable )
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

            IndexList equivStates;
            equivStates.push_back( std::pair<int,int>( baseState, 0 ) );
            int matrixRow = 1;

            std::list<int>::iterator it = stateList.begin();
            while( it != stateList.end() )
            {
                if( areOutputsEqual( baseState, *it ) )
                {
                    equivStates.push_back( std::pair<int,int>( *it, matrixRow++ ) );
                    it = stateList.erase( it );
                }
                else
                    ++it;
            }

            EquivClass *ec = new EquivClass;
            ec->equivStateL = equivStates;

            equivClassV.push_back( ec );
        }

        for( size_t i = 0; i < equivClassV.size(); ++i )
            createIndexesForClass( equivClassV.at( i ) );
    }

    void print( void )
    {
        for( size_t i = 0; i < equivClassV.size(); ++i )
        {
            EquivClass *ec = equivClassV.at( i );
            std::cout << std::endl << "Equiv class " << i + 1 << std::endl;

            IndexList::iterator it = ec->equivStateL.begin();
            while( it != ec->equivStateL.end() )
            {
                std::cout << it->first + 1 << ' ';
                for( size_t j = 0; j < ptrNextM->numCols(); ++j )
                    std::cout << ptrNextM->operator[]( it->first )[ j ] + 1 << ec->classIdxM[ it->second ][ j ] << ' ';
                std::cout << std::endl;

                ++it;
            }
        }
    }

    KTable *refinePartition()
    {
        KTable *refined = new KTable;

        for( size_t i = 0; i < equivClassV.size(); ++i )
        {
            EquivClass *ec = equivClassV.at( i );
            IndexList inCheckList = ec->equivStateL;

            while( inCheckList.empty() == false )
            {
                std::pair<int,int> baseState = inCheckList.front();
                int matrixRow = 1;
                inCheckList.pop_front();

                IndexList equivList;
                equivList.push_back( std::pair<int,int>( baseState.first, 0 ) );

                IndexList::iterator it = inCheckList.begin();

                while( it != inCheckList.end() )
                {
                    if( areNextStatesEqual( &baseState, &(*it), &ec->classIdxM )  )
                    {
                        equivList.push_back( std::pair<int,int>( it->first, matrixRow++ ) );
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

            for( size_t i = 0; i < refined->equivClassV.size(); ++i )
                refined->createIndexesForClass( refined->equivClassV.at( i ) );
        }
        else
        {
            delete refined;
            refined = 0;
        }

        return refined;
    }

    void clear( void )
    {
        for( size_t i = 0; i < equivClassV.size(); ++i )
            delete equivClassV.at( i );
        equivClassV.clear();
    }

    ~KTable()
    {
        clear();
    }

private:

    bool areNextStatesEqual( std::pair<int,int> *s1, std::pair<int,int> *s2, Matrix<char> *ptrClassM )
    {
        bool equal = true;
        for( size_t j = 0; j < ptrClassM->numCols(); ++j )
        {
            if( ptrClassM->operator[]( s1->second )[ j ] != ptrClassM->operator[]( s2->second )[ j ] )
            {
                equal = false;
                break;
            }
        }
        return equal;
    }

    bool areOutputsEqual( int s1, int s2 )
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

    void createIndexesForClass( EquivClass *ec )
    {
        ec->classIdxM.setSize( ec->equivStateL.size(), ptrOutM->numCols() );

        IndexList::iterator it, end;
        it = ec->equivStateL.begin();
        end = ec->equivStateL.end();

        char startChar = 'a';
        size_t rowIdx = 0;

        while( it != end )
        {
            int sIdx = it->first;

            for( size_t j = 0; j < ptrNextM->numCols(); ++j )
            {
                int classIdx = getClassIdxByState( ptrNextM->operator[]( sIdx )[ j ] );
                ec->classIdxM[ rowIdx ][ j ] = startChar + classIdx;
            }

            ++rowIdx;
            ++it;
        }
    }

    int getClassIdxByState( int sIdx )
    {
        for( size_t i = 0; i < equivClassV.size(); ++i )
        {
            EquivClass *ec = equivClassV.at( i );
            IndexList::iterator it, end;
            it = ec->equivStateL.begin();
            end = ec->equivStateL.end();

            while( it != end )
            {
                if( it->first == sIdx ) return (int)i;
                ++it;
            }
        }

        return -1;
    }

private:
    std::vector<EquivClass*> equivClassV;
    Matrix<int> *ptrOutM;
    Matrix<int> *ptrNextM;
};

class Refiner
{
public:
    Refiner(){}

    void processDeltaTable( DeltaTable *deltaTable )
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

    void clear()
    {
        while( ktableL.empty() == false )
        {
            delete ktableL.front();
            ktableL.pop_front();
        }
    }

    ~Refiner()
    {
        clear();
    }

private:
    std::list<KTable*> ktableL;
};


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

    table.print();

    return 0;
}
