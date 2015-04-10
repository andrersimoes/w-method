#include "TransitionTable.h"
#include "input.h"

void TransitionTable::addInput( Input *input )
{
    _alphabet[ input->getId() ] = input;
}

void TransitionTable::clearAlphabet( void )
{
    std::map<char,Input*>::iterator it;

    while( it != _alphabet.end() )
    {
        delete it->second;
        ++it;
    }

    _alphabet.clear();
}

Input *TransitionTable::getInputById( char id )
{
    std::map<char,Input*>::iterator it = _alphabet.find( id );
    if( it != _alphabet.end() ) 
        return it->second;
    else
    {
        std::cout << "TransitionTable::getInputById() - can't find input for char " << id << std::endl;
        throw;
    }
}

void TransitionTable::loadInputsFromFile( std::string filename )
{
    std::ifstream file;
    file.open( filename.c_str() );

    if( !file.is_open() )
    {
        std::cout << "TransitionTable::loadInputsFromFile() - cant open/read file " << file << std::endl;
        throw;
    }

    std::string strHeader; /// eliminate header and comments
    std::getline( file, strHeader );

    while( !file.eof() )
    {
        char id;
        std::string stdType;
        double value, timeToReference, timeOnSteady;

        file >> id;

        if( file.eof() )
            break;

        file >> stdType;
        file >> value;
        file >> timeToReference;
        file >> timeOnSteady;

        Input::Type type;

        if( stdType == "Step" || stdType == "step" )
            type = Input::IN_STEP;
        else if( stdType == "Ramp" || stdType == "ramp" )
            type = Input::IN_RAMP;

        Input *input = new Input( id );
        input->setType( type  );
        input->set( value, timeToReference, timeOnSteady );
        _alphabet[ id ] = input;
    }

    file.close();
}

TransitionTable::~TransitionTable()
{
}
