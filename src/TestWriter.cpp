#include "TestWriter.h"

TestWriter::TestWriter( DynamicTransitionTable<Output> *ptable, std::list<std::string> *ptrTestList )
{
    ptrTable = ptable;
    ptrTestCaseL = ptrTestList;
    ptrNextMat = ptrTable->getNextStateMatrixPtr();
    ptrOutMat = ptrTable->getOutMatrixPtr();
    firstStateIdx = 0;
} 

void TestWriter::writeKeywordTest( std::ostream &stream )
{
    std::list<std::string>::iterator itTestCase, end;
    itTestCase = ptrTestCaseL->begin();
    end = ptrTestCaseL->end();

    while( itTestCase!= end )
    {
        stream << "sat.changeDirectionTo;";
        //std::cout << std::endl << *itTestCase << std::endl;

        writeSignalType( stream, *itTestCase );
        writeAngularReference( stream, *itTestCase );
        writeStartTime( stream, *itTestCase );
        writeTimeToReference( stream, *itTestCase );

        writeMaxSteadyError( stream, *itTestCase );

        ++itTestCase;
    }
}

void TestWriter::writeAngularReference( std::ostream &stream, std::string &inputSequence )
{
    stream << "angularReference [ ";
    std::string::iterator itChar = inputSequence.begin();
    std::string::iterator endInput = inputSequence.end();

    while( itChar != endInput )
    {
        Input *input = ptrTable->getInputById( *itChar );
        stream << input->getValue() << " ";

        ++itChar;
    }
    stream << "] " << std::endl;
}

void TestWriter::writeMaxSteadyError( std::ostream &stream, std::string &inputSequence )
{
    stream << "maxSteadyError [ ";
    std::string::iterator itChar = inputSequence.begin();
    std::string::iterator endInput = inputSequence.end();

    int stateIdx = firstStateIdx;

    while( itChar != endInput )
    {
        size_t colIdx = (size_t)( *itChar - 'a' );

        Output &out = ptrOutMat->operator[]( stateIdx )[ colIdx ];
        stream << out.getError() << " ";

        stateIdx = ptrNextMat->operator[]( stateIdx )[ colIdx ];
        ++itChar;
    }
    stream << "] " << std::endl;
}

void TestWriter::writeSignalType( std::ostream &stream, std::string &inputSequence )
{
    stream << "\"signalType [ ";
    std::string::iterator itChar = inputSequence.begin();
    std::string::iterator endInput = inputSequence.end();

    while( itChar != endInput )
    {
        Input *input = ptrTable->getInputById( *itChar );
        switch( input->getType() )
        {
            case Input::IN_STEP: stream << "Step "; break;
            case Input::IN_RAMP: stream << "Ramp "; break;
            default:
            {
                std::cout << "TestWriter::writeKeywordTest() - bad input type " << std::endl;
                throw;
                break;
            }
        };

        ++itChar;
    }
    stream << "] " << std::endl;
}

void TestWriter::writeStartTime( std::ostream &stream, std::string &inputSequence )
{
    stream << "startTime [ 0 ";
    std::string::iterator itChar = inputSequence.begin();
    std::string::iterator endInput = inputSequence.end();

    if( itChar != endInput ) --endInput;

    double startTime = 0;

    while( itChar != endInput )
    {
        Input *input = ptrTable->getInputById( *itChar );
        startTime += input->getTimeToReference() + input->getTimeOnSteady();
        stream << startTime << " ";

        ++itChar;
    }
    stream << "] " << std::endl;
}

void TestWriter::writeTimeToReference( std::ostream &stream, std::string &inputSequence )
{
    stream << "timeToReference [ ";
    std::string::iterator itChar = inputSequence.begin();
    std::string::iterator endInput = inputSequence.end();

    while( itChar != endInput )
    {
        Input *input = ptrTable->getInputById( *itChar );
        stream << input->getTimeToReference() << " ";

        ++itChar;
    }
    stream << "]\";";
}
