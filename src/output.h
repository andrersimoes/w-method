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
    Output( Type responseType, double ref, double transErr, double steadyErr ){ type = responseType, reference = ref; steadyError = steadyErr; transientError = transErr; }

    double getReference( void ){ return reference; }
    double getSteadyError( void ){ return steadyError; }
    double getTransientError( void ){ return transientError; }
    Type getType( void ){ return type; }

    static void setGlobalError( double e ){ globalError = e; }
    std::string strType( void );

    Output & operator = ( const Output &b ){ type = b.type; reference = b.reference; transientError = b.transientError; steadyError = b.steadyError; return *this; }

    friend bool operator == ( const Output &a, const Output &b );
    friend bool operator != ( const Output &a, const Output &b );
    friend std::istream & operator >> ( std::istream &, Output & );
    friend std::ostream & operator << ( std::ostream &, const Output & );
private:
    static double globalError;

    Type type;
    double reference;
    double steadyError;
    double transientError;
};

#endif // OUTPUT_H
