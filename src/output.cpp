#include "output.h"

#include <cmath>
#include <cstdlib>

double Output::globalError = 0.0000001;

bool operator == ( const Output &a, const Output &b )
{
    if( a.type != b.type ) return false;

    double refError = fabs( a.reference - b.reference );
    double errorError = fabs( a.steadyError - b.steadyError );

    if( refError <= Output::globalError && errorError <= Output::globalError )
        return true;
    else
        return false;
}

bool operator != ( const Output &a, const Output &b )
{
    if( a.type != b.type ) return true;

    double refError = fabs( a.reference - b.reference );
    double errorError = fabs( a.steadyError - b.steadyError );

#ifdef DEBUG
    std::string aType = ( (a.type == Output::OUT_STEP) ? "step" : "ramp" );
    std::string bType = ( (b.type == Output::OUT_STEP) ? "step" : "ramp" );
#endif

    if( refError > Output::globalError || errorError > Output::globalError )
        return true;
    else
        return false;
}


std::istream & operator >> ( std::istream &stream, Output &out )
{
    /*char c;
    std::string strType;

    std::string buffer;
    stream >> c; // remove {
    stream >> strType;
    strType.erase( strType.end() - 1 );
    stream >> out.value;
    stream >> c; // remove ,
    stream >> out.error;
    stream >> buffer; // remove {*/

    int paramCounter = 0;
    bool curlyBrackets = false;

    char c;
    std::string buffer;
    while( stream.get( c ) )
    {
        //std::cout << c << " " << (int) c << std::endl;
        if( c == '{' && !curlyBrackets ) 
        {
            curlyBrackets = true;
            continue;
        }
        else if( c == '}' && curlyBrackets )
        {
            if( ! buffer.empty() && paramCounter == 3 )
                out.steadyError = atof( buffer.c_str() );
            else
            {
                std::cout << "Output::operator >>() - " \
                    "incomplete data " << buffer << std::endl;
                throw;
            }

            break;
        }
        else
        {
            if( c == ',' )
            {
                std::string errorMsg = "Output::operator >>() - bad data after value ";
                switch( paramCounter )
                {
                    case 0:
                    {
                        if( buffer.empty() == false )
                        {
                            if( buffer == "step" ) out.type = Output::OUT_STEP;
                            else if( buffer == "ramp" ) out.type = Output::OUT_RAMP;
                            else
                            {
                                std::cout << "Output::operator >>() - bad type = \"" << buffer << "\""<< std::endl;
                                throw;
                            }

                            buffer.clear();
                            ++paramCounter;
                        }
                        else
                        {
                            std::cout << errorMsg << buffer << std::endl;
                            throw;
                        }
                        break;
                    }
                    case 1:
                    {
                        if( buffer.empty() == false )
                        {
                            out.reference = atof( buffer.c_str() ); ++paramCounter;
                            buffer.clear();
                        }
                        else
                        {
                            std::cout << errorMsg << buffer << std::endl;
                            throw;
                        }

                        break;
                    }
                    case 2:
                    {
                        if( buffer.empty() == false )
                        {
                            out.transientError = atof( buffer.c_str() ); ++paramCounter;
                            buffer.clear();
                        }
                        else
                        {
                            std::cout << errorMsg << buffer << std::endl;
                            throw;
                        }
                        break;
                    }
                    default:
                    {
                        std::cout << "Output::operator >>() - unexpected param counter value = " << paramCounter << std::endl;
                        throw;
                    }
                }
            }
            else
            {
                // Enter char = 10
                if( c != ' ' && c != 10 ) buffer += c;
            }
        }
    }

    return stream;
}

std::ostream & operator << ( std::ostream &stream, const Output &out )
{
    std::string strType = ( (out.type == Output::OUT_STEP) ? "step" : "ramp" );
    stream << "{ " << strType << ", " << out.reference << ", " << out.transientError << ", " <<  out.steadyError << " }";
    return stream;
}

