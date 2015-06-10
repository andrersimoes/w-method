#include "TestWriter.h"

TestWriter::TestWriter( DynamicTransitionTable<Output> *ptable, std::list<std::string> *ptrTestList )
{
    ptrTable = ptable;
    ptrTestCaseL = ptrTestList;
    ptrNextMat = ptrTable->getNextStateMatrixPtr();
    ptrOutMat = ptrTable->getOutMatrixPtr();
    firstStateIdx = 0;
} 

///
/// OBS: maxTransientError esta relacionado ao regime transiente
// do sistema e carrega um valor máximo de erro de posicao angular.
void TestWriter::loadConstraints( std::string filename )
{
    std::ifstream file( filename.c_str() );

    if( !file.is_open() )
    {
        std::cout << "TestWriter::loadConstraints() - could not open file " << filename << std::endl;
        throw;
    }

    std::string tmp;
    file >> tmp; // sat.momentOfInertia
    file >> satMomentOfInertia;
    file >> tmp; // maxRwSpeed string
    file >> maxRwSpeed;
    file >> tmp; // maxTransientSpeed string
    file >> maxTransientSpeed;
    file >> tmp; // rateToStepReference
    file >> rateToStepReference;
    file >> tmp; // timeToStepReference
    file >> timeToStepReference;

    file.close();
}

void TestWriter::writeKeywordTest( std::ostream &stream )
{
    stream << "Keyword;Input;Expected Output;Comments" << std::endl;
    stream << "sat.setMomentOfInertia;" << satMomentOfInertia << std::endl;
    stream << "SetRateToStepReference;" << rateToStepReference << std::endl;
    stream << "SetTimeToStepReference;" << timeToStepReference << std::endl;

    std::list<std::string>::iterator itTestCase, end;
    itTestCase = ptrTestCaseL->begin();
    end = ptrTestCaseL->end();

    int tcNumber = 1;

    while( itTestCase!= end )
    {
        writeSetSimTime( stream, *itTestCase );
        stream << "sat.changeDirectionTo;";
        //std::cout << std::endl << *itTestCase << std::endl;

        writeSignalType( stream, *itTestCase );
        writeAngularReference( stream, *itTestCase );
        writeStartTime( stream, *itTestCase );
        writeTimeToReference( stream, *itTestCase );

        stream << "\"maxRwSpeed " << maxRwSpeed << std::endl;
        writeMaxTransientError( stream, *itTestCase );
        writeMaxSteadyError( stream, *itTestCase );
        stream << "maxTransientSpeed " << maxTransientSpeed << "\";";
        stream << "Test case " << tcNumber++ << " - " << *itTestCase << std::endl;

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
    stream << "]" << std::endl;
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
        stream << out.getSteadyError() << " ";

        stateIdx = ptrNextMat->operator[]( stateIdx )[ colIdx ];
        ++itChar;
    }
    stream << "]" << std::endl;
}

void TestWriter::writeMaxTransientError( std::ostream &stream, std::string &inputSequence )
{
    stream << "maxTransientError [ ";
    std::string::iterator itChar = inputSequence.begin();
    std::string::iterator endInput = inputSequence.end();

    int stateIdx = firstStateIdx;

    while( itChar != endInput )
    {
        size_t colIdx = (size_t)( *itChar - 'a' );

        Output &out = ptrOutMat->operator[]( stateIdx )[ colIdx ];
        stream << out.getTransientError() << " ";

        stateIdx = ptrNextMat->operator[]( stateIdx )[ colIdx ];
        ++itChar;
    }
    stream << "]" << std::endl;
}

void TestWriter::writeSetSimTime( std::ostream &stream, std::string &inputSequence )
{
    std::string::iterator itChar = inputSequence.begin();
    std::string::iterator endInput = inputSequence.end();

    double simTime = 0;
    if( itChar != endInput ) --endInput;

    while( itChar != endInput )
    {
        Input *input = ptrTable->getInputById( *itChar );
        simTime += input->getTimeToReference() + input->getTimeOnSteady();

        ++itChar;
    }

    if( itChar != inputSequence.end() )
    {
        Input *input = ptrTable->getInputById( *itChar );
        simTime += input->getTimeToReference() + input->getTimeOnSteady();
    }

    std::cout << "SetSimTime;" << simTime << std::endl;
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
    stream << "]" << std::endl;
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
    stream << "]" << std::endl;
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
