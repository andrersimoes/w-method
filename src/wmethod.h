#ifndef WMETHOD_H
#define WMETHOD_H

#include <list>
#include <string>

class TransitionTable;

class WMethod
{
public:
    WMethod();
    void processTransitionTable( TransitionTable *tt, std::list<std::string> *ptrTestCaseList, bool fullDiagSet = false );
};

#endif //WMETHOD_H
