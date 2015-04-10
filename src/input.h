#ifndef INPUT_H
#define INPUT_H

#include <iostream>

class Input
{
public:
    enum Type
    {
        IN_RAMP,
        IN_STEP,
    };

    Input(){}
    Input( char charId ){ id = charId; }

    char getId( void ){ return id; }
    double getValue( void ){ return value; }
    Type getType( void ){ return type; }

    double getTimeOnSteady( void ){ return timeOnSteady; }
    double getTimeToReference( void ){ return timeToReference; }

    void set( double val, double refPeriod, double steadyPeriod );
    void setType( Type t ){ type = t; }

    friend std::istream & operator >> ( std::istream &stream, Input &in );
    friend std::ostream & operator << ( std::ostream &stream, const Input &in );
private:
    char id;
    Type type;

    double value;
    double timeToReference;
    double timeOnSteady;
};

#endif // INPUT_H
