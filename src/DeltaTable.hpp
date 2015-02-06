template<typename OutType>
inline
void DeltaTable<OutType>::resizeMatrices( void )
{
    _outputMat.setSize( _numStates, _alfabet.size() );
    _nextStateMat.setSize( _numStates, _alfabet.size() ); 
}

template<typename OutType>
inline
void DeltaTable<OutType>::print( void )
{
    std::set<std::string>::iterator alfaStart, alfaIt, alfaEnd;
    alfaStart = _alfabet.begin();
    alfaEnd = _alfabet.end();

    std::cout << " state | ";
    for( unsigned short i = 0; i < 2; ++i )
    {
        alfaIt = alfaStart;
        while( alfaIt != alfaEnd )
            std::cout << *alfaIt++ << " | ";
    }
    std::cout << std::endl;

    for( size_t i = 0; i < _numStates; ++i )
    {
        std::cout << "   " << i << "   | ";

        for( size_t j = 0; j < _outputMat.numCols(); ++j ) 
            std::cout << "  " << _outputMat[ i ][ j ] << "  | ";

        std::cout << "|";

        for( size_t j = 0; j < _nextStateMat.numCols(); ++j ) 
            std::cout << "  " << _nextStateMat[ i ][ j ] << "  | ";

        std::cout << std::endl;
    }
}
