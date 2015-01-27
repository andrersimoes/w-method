#ifndef REFINER_H
#define REFINER_H

#include <string>
#include <vector>

class DeltaTable;
class KTable;

class Refiner
{
public:
    Refiner();

    void buildDiagSet( void ); // TODO
    void clear();
    bool isMachineMinimal( void );
    void processDeltaTable( DeltaTable *dt );

    ~Refiner();
private:
    std::string buildDiagSequence( int s1, int s2 );

    DeltaTable *deltaTable;
    std::vector<KTable*> ktableV;
};

#endif // REFINER_H
