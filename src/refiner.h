#ifndef REFINER_H
#define REFINER_H

#include <list>
#include <string>
#include <vector>

class DeltaTable;
class KTable;

class Refiner
{
public:
    Refiner();

    std::list<std::string> buildDiagSet( bool full = false ); // TODO
    void clear();
    bool isMachineMinimal( void );
    void processDeltaTable( DeltaTable *dt );

    ~Refiner();
private:
    void buildFullDiagSet( std::list<std::string> &retval );
    void buildUniqueStringSet( std::list<std::string> &retval ); //characterization set
    void keepGreaterElementOnList( std::string &strElement,
        std::list<std::string> &list );

    std::string buildDiagSequence( int s1, int s2 );

    DeltaTable *deltaTable;
    std::vector<KTable*> ktableV;
};

#endif // REFINER_H
