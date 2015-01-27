#ifndef KTABLE_H
#define KTABLE_H

#include <list>
#include <vector>

#include "matrix.h"

struct EquivClass
{
    std::list<int> equivStateL;
};

class DeltaTable;

class KTable
{
public:
    KTable();

    void buildFirstPartition( DeltaTable *deltaTable );
    void clear( void );
    void print( void ); 
    KTable *refinePartition();

    ~KTable();
private:
    bool areNextStatesEqual( int s1, int s2 );
    bool areOutputsEqual( int s1, int s2 );
    void createIndexesForClass( EquivClass *ec );
    void createTableIndexes( void );
    int getClassIdxByState( int sIdx );

private:
    std::vector<EquivClass*> equivClassV;
    Matrix<int> *ptrOutM;
    Matrix<int> *ptrNextM;
    Matrix<char> classIdxM;
};

#endif // KTABLE_H
