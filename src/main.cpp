#include <iostream>
#include <set>
#include <vector>
#include <list>
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

class EquivalenceK
{
public:
    EquivalenceK( int kLevel = 1 )
    {
        _kLevel = kLevel;
    }

    void processDeltaTable( DeltaTable &table, std::list<int> inCheckList )
    {
        _partition.clear();

        _ptrOutM = table.getOutMatrixPtr();
        _ptrNextStateM = table.getNextStateMatrixPtr();

        std::list<int>::iterator it, mainState;
        it = mainState = inCheckList.begin();
        ++it;

        std::list<int> notEquivList;

        while( ! inCheckList.empty() )
        {
            int firstSt = inCheckList.front();

            std::list<int> equivL;
            equivL.push_back( firstSt );
            inCheckList.pop_front();

            std::list<int>::iterator it = inCheckList.begin();
            while( it != inCheckList.end() )
            {
                bool isEquivalent = areStatesEquivalent( firstSt, *it, _kLevel );
                
                if( isEquivalent )
                {
                    equivL.push_back( *it );
                    it = inCheckList.erase( it );
                }
                else
                {
                    notEquivList.push_back( *it );
                    it = inCheckList.erase( it );
                }
            }

            if( ! equivL.empty() ) _partition.push_back( equivL );
            inCheckList = notEquivList;
            notEquivList.clear();
        }
    }

    void print()
    {
        std::cout << "EquivalenceK::print() - " << _kLevel << "-equivalence partition" << std::endl;

        std::list< std::list<int> >::iterator itOut;
        std::list<int>::iterator itIn;

        itOut = _partition.begin();

        while( itOut != _partition.end() )
        {
            itIn = itOut->begin();

            while( itIn != itOut->end() )
            {
                std::cout << *itIn << " ";
                ++itIn;
            }
            std::cout << std::endl;

            ++itOut;
        }
    }

private:
    bool areStatesEquivalent( int sa, int sb, int level )
    {
        bool isEquivalent = true;
        for( size_t j = 0; j < _ptrOutM->numCols(); ++j )
        {
            if( _ptrOutM->operator[]( sa )[j] != _ptrOutM->operator[]( sb )[j] )
            {
                isEquivalent = false;
                break;
            }
        }

        if( isEquivalent && level > 1 )
        {
            for( size_t j = 0; j < _ptrOutM->numCols(); ++j )
            {
                if( !areStatesEquivalent( _ptrNextStateM->operator[]( sa )[j],
                                          _ptrNextStateM->operator[]( sb )[j],
                                          level-1 ) )
                {
                    isEquivalent = false;
                    break;
                }
            }
        }

        return isEquivalent;
    }

    std::list< std::list<int> > _partition;

    Matrix<int> *_ptrOutM;
    Matrix<int> *_ptrNextStateM;

    size_t _kLevel;
};

class PartitionK
{
public:
    PartitionK(){}

    void processDeltaTable( DeltaTable *table )
    {
        //EquivalenceK *equiv = new EquivalenceK;
    }
private:

    //std::list<EquivalenceK*> _equivL;
};

int main()
{
    DeltaTable table;

    table.addAction( "alfa" );
    table.addAction( "beta" );
    table.addAction( "gamma" );
    table.setNumberOfStates( 9 );

    table.resizeMatrices();

    Matrix<int> &outM = table.getOutMatrixRef();
    Matrix<int> &stateM = table.getNextStateMatrixRef();

    outM.loadFromFile( "../data/outTable.txt", true ); 
    stateM.loadFromFile( "../data/nextTable.txt", true ); stateM += -1;

    table.print();


    std::list<int> inCheckList;
    for( size_t i = 0; i < outM.numRows(); ++i ) inCheckList.push_back( i );

    EquivalenceK ek(10);
    ek.processDeltaTable( table, inCheckList );
    ek.print();

    return 0;
}
