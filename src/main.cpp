#include <iostream>
#include <cmath>
#include <cstdlib>

#include "DeltaTable.h"

class Output
{
public:
    Output(){}
    Output( double val, double err ){ value = val; error = err; }

    static void setGlobalError( double e ){ globalError = e; }

    Output & operator = ( const Output &b ){ value = b.value; error = b.error; return *this; }

    friend bool operator == ( const Output &a, const Output &b );
    friend bool operator != ( const Output &a, const Output &b );
    friend std::istream & operator >> ( std::istream &, Output & );
    friend std::ostream & operator << ( std::ostream &, const Output & );
private:
    static double globalError;

    double value;
    double error;
};

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

int main()
{
    DeltaTable<Output> delta;
    delta.addAction( "a" );
    delta.addAction( "b" );
    delta.addAction( "c" );
    delta.addAction( "d" );
    delta.addAction( "e" );
    delta.addAction( "f" );

    delta.setNumberOfStates( 4 );
    delta.resizeMatrices();

    Matrix<Output> &outM = delta.getOutMatrixRef();
    Matrix<int> &nextM = delta.getNextStateMatrixRef();

    nextM.loadFromFile( "../data/nextTable-sat01.txt" );
    outM.loadFromFile( "../data/outTable-sat01.txt" );

    nextM.print();
    outM.print();


    /*delta.setNumberOfStates( 3 );
    delta.resizeMatrices();
    delta.print();*/

    return 0;
}
