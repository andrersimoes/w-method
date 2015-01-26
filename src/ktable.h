#ifndef KTABLE_H
#define KTABLE_H

#include <list>
#include <vector>

#include "matrix.h"

typedef std::list< std::pair<int,int> > IndexList; // <numero do estado, numero da linha correspondente em classIdxM>

struct EquivClass
{
    IndexList equivStateL;
    Matrix<char> classIdxM;
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
    bool areNextStatesEqual( std::pair<int,int> *s1, std::pair<int,int> *s2, Matrix<char> *ptrClassM );
    bool areOutputsEqual( int s1, int s2 );
    void createIndexesForClass( EquivClass *ec );
    int getClassIdxByState( int sIdx );

private:
    std::vector<EquivClass*> equivClassV;
    Matrix<int> *ptrOutM;
    Matrix<int> *ptrNextM;
};

#endif // KTABLE_H
