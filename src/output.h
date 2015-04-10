#ifndef OUTPUT_H
#define OUTPUT_H

#include <iostream>

class Output
{
public:
    enum Type
    {
        OUT_RAMP,
        OUT_STEP,
    };

    Output(){}
    Output( Type responseType, double val, double err ){ type = responseType, value = val; error = err; }

    Type getType( void ){ return type; }
    double getError( void ){ return error; }
    double getValue( void ){ return value; }

    static void setGlobalError( double e ){ globalError = e; }
    std::string strType( void );

    Output & operator = ( const Output &b ){ type = b.type; value = b.value; error = b.error; return *this; }

    friend bool operator == ( const Output &a, const Output &b );
    friend bool operator != ( const Output &a, const Output &b );
    friend std::istream & operator >> ( std::istream &, Output & );
    friend std::ostream & operator << ( std::ostream &, const Output & );
private:
    static double globalError;

    Type type;
    double value;
    double error;
};

#endif // OUTPUT_H
