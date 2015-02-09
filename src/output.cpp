#include "output.h"

#include <cmath>
#include <cstdlib>

double Output::globalError = 0.0000001;

bool operator == ( const Output &a, const Output &b )
{
    double valueError = fabs( a.value - b.value );
    double errorError = fabs( a.error - b.error );

    if( valueError <= Output::globalError && errorError <= Output::globalError )
        return true;
    else
        return false;
}

bool operator != ( const Output &a, const Output &b )
{
    double valueError = fabs( a.value - b.value );
    double errorError = fabs( a.error - b.error );

    if( valueError > Output::globalError || errorError > Output::globalError )
        return true;
    else
        return false;
}


std::istream & operator >> ( std::istream &stream, Output &out )
{
    /*std::string buffer;
    stream >> c; // remove {
    stream >> out.value;
    stream >> c; // remove ,
    stream >> out.error;
    stream >> buffer; // remove {*/

    int numberCounter = 0;
    bool curlyBrackets = false;

    char c;
    std::string buffer;
    while( stream.get( c ) )
    {
        if( c == '{' && !curlyBrackets ) 
        {
            curlyBrackets = true;
            continue;
        }
        else if( c == '}' && curlyBrackets )
        {
            if( ! buffer.empty() && numberCounter == 1 )
                out.error = atof( buffer.c_str() );
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
                if( numberCounter == 0 && buffer.empty() == false )
                {
                    out.value = atof( buffer.c_str() ); ++numberCounter;
                    buffer.clear();
                }
                else
                {
                    std::cout << "Output::operator >>() - bad number after value " 
                        << buffer << std::endl;
                    throw;
                    break;
                }
            }
            else
                if( c != ' ' ) buffer += c;
        }
    }

    return stream;
}

std::ostream & operator << ( std::ostream &stream, const Output &out )
{
    stream << "{ " << out.value << ", " << out.error << " }";
    return stream;
}

