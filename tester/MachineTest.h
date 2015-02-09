#ifndef MACHINETEST_H
#define MACHINETEST_H

#include <string>
#include <list>

class TransitionTable;

struct MachineTest
{
    MachineTest();

    bool minimal;
    size_t numStates;
    size_t numRefinementClasses;
    std::list<std::string> strActionL;
    std::list<std::string> wL;

    void clear( void );
    void configure( TransitionTable *tt );
    void load( std::string filename );
    void print( void );
};

#endif /// MACHINETEST_H
