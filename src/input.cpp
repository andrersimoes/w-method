#include "input.h"

void Input::set( double val, double refPeriod, double steadyPeriod )
{
    value = val;
    timeToReference = refPeriod;
    timeOnSteady = steadyPeriod;
}

std::ostream & operator << ( std::ostream &stream, const Input &in )
{
    std::string strType = ( in.type == Input::IN_STEP ) ? "step" : "ramp";
    stream << "input( " << in.id << ", " << strType << ", " << in.value << ", " << in.timeToReference << ", " << in.timeOnSteady << " )";
    return stream;
}

std::istream & operator >> ( std::istream &stream, Input &in )
{
    std::string stdType;

    stream >> in.id;
    stream >> stdType;
    stream >> in.value;
    stream >> in.timeToReference;
    stream >> in.timeOnSteady;

    if( stdType == "Step" || stdType == "step" )
        in.type = Input::IN_STEP;
    else if( stdType == "ramp" || stdType == "ramp" )
        in.type = Input::IN_RAMP;

    return stream;
}


