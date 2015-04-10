template<typename OutType>
inline
bool DynamicTransitionTable<OutType>::areOutputsEqual( int s1, int s2, size_t *retvalActionIdx )
{
    bool equal = true;
    size_t cols = _outputMat.numCols();
    for( size_t j = 0; j < cols; ++j )
    {
        if( _outputMat[ s1 ][ j ] != _outputMat[ s2  ][ j ] )
        {
            if( retvalActionIdx ) *retvalActionIdx = j;
            equal = false;
            break;
        }
    }
    return equal;
}

template<typename OutType>
inline
void DynamicTransitionTable<OutType>::resizeMatrices( void )
{
    _outputMat.setSize( _numStates, _alphabet.size() );
    _nextStateMat.setSize( _numStates, _alphabet.size() ); 
}

template<typename OutType>
inline
void DynamicTransitionTable<OutType>::print( void )
{
    std::map<char,Input*>::iterator alfaStart, alfaIt, alfaEnd;
    alfaStart = _alphabet.begin();
    alfaEnd = _alphabet.end();

    std::cout << " state | ";
    for( unsigned short i = 0; i < 2; ++i )
    {
        alfaIt = alfaStart;
        while( alfaIt != alfaEnd )
        {
            std::cout << alfaIt->second->getId() << " | ";
            ++alfaIt;
        }
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

