#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

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

#endif // OUTPUT_H
