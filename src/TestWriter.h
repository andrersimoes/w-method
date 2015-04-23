#ifndef TESTWRITER_H
#define TESTWRITER_H

#include <list>
#include <string>

#include "DynamicTransitionTable.h"
#include "output.h"

class TestWriter
{
public:
    TestWriter( DynamicTransitionTable<Output> *ptable, std::list<std::string> *ptrTestList );

    void writeKeywordTest( std::ostream &stream );
    void loadConstraints( std::string filename );

private:
    void writeSetSimTime( std::ostream&, std::string& );

    void writeSignalType( std::ostream&, std::string& );
    void writeAngularReference( std::ostream&, std::string& );
    void writeStartTime( std::ostream&, std::string& );
    void writeTimeToReference( std::ostream&, std::string& );

    void writeMaxSteadyError( std::ostream&, std::string& );

    int firstStateIdx;
    DynamicTransitionTable<Output> *ptrTable;
    Matrix<int> *ptrNextMat;
    Matrix<Output> *ptrOutMat;
    std::list<std::string> *ptrTestCaseL;

    double maxRwSpeed;
    double maxTransientError;
    double maxTransientSpeed;
    double rateToStepReference;
    double satMomentOfInertia;
};

#endif // TESTWRITER_H
