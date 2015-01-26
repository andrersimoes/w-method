#ifndef REFINER_H
#define REFINER_H

#include <list>

class DeltaTable;
class KTable;

class Refiner
{
public:
    Refiner();

    void clear();
    void processDeltaTable( DeltaTable *deltaTable );

    ~Refiner();
private:
    std::list<KTable*> ktableL;
};

#endif // REFINER_H
