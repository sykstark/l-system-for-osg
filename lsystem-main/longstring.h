#ifndef LONGSTRING_H_
#define LONGSTRING_H_

#include <string>
#include "utils.h"

namespace AP_LSystem {
static unsigned int ZERO = 0;

/**
  * Class for storing L-system string in fast form optimalized for append operation and for
  * storing values/parameters of different types
  */
class LongString
{
private:
    char * m_String;                ///< buffer witch contains all LongString data
    unsigned int m_Length;          ///< length of string in bytes
    unsigned int m_Allocated;       ///< number of allocated bytes
    unsigned int m_Increment;       ///< increment of LongString allocated bytes

    //@{
    /**
      * Method for determination of type of parameter in Parameter::Type type
      * @return type of parameter
      */
    inline Parameter::Type getType(double) { return Parameter::LS_DOUBLE; }
    inline Parameter::Type getType(double *) { return Parameter::LS_DOUBLE; }
    inline Parameter::Type getType(unsigned int) { return Parameter::LS_UINT; }
    inline Parameter::Type getType(unsigned char) { return Parameter::LS_UBYTE; }
    inline Parameter::Type getType(int) { return Parameter::LS_INT; }
    inline Parameter::Type getType(int *) { return Parameter::LS_INT; }
    //@}

    /**
      * Resizes allocated memory of LongString by incrementing it by m_Increment
      */
    void resize();

    /**
      * Append std::string to end of LongString
      * @param str string to append
      */
    void append( std::string str );

    /**
      * Append parameter type
      * @param type parameter type
      */
    void append( Parameter::Type type );
public:
    /**
      * Create a LongString and set a default increment and initial length
      * @param length initial length and also default increment
      */
    LongString( unsigned int length = 1048576); // 2^20 bytes - 1 MB

    /**
      * Copy constructor
      */
    LongString( const LongString& c );

    /**
      * Assignment operator
      */
    LongString& operator=( const LongString & c );

    /**
      * Destructor
      */
    ~LongString( );
    
    /**
      * Converts standart string of L-system to internal LongString format. Useful
      * for successors and axioms.
      */
    void convertFromString( std::string * str, unsigned int & = ZERO, const char = '\0' );
	
    /**
      * Appends some parameter of any type declared in Parameter::Type. Can be used to
      * append module parameter.
      * @param parameter
      */
	template< class T >
	void append( T par )
	{
        if(m_Allocated < m_Length + sizeof( T ) + 2)
		{
			resize( );
		}
		append( getType(par) );
        memcpy( m_String + m_Length, &par, sizeof( T ) );
        m_Length += sizeof( T );
		append( getType(par) );
	}

    /**
      * Appends one byte.
      * @param byte to append
      */
	void append( const char ch );
	

    /**
      * Appends buffer with data.
      * @param data buffer with data
      * @param length length of buffer
      */
    void append( const char * data, int length);

    /**
      * Appends another LongString
      * @param ls string to append
      */
    void append( LongString * ls);

    /**
      * Extracts parameters of same type from some position in string
      * @param ls string to append
      */
	template< class T >
	bool getParameters( unsigned int & pos, T * pParams, int & paramsCnt )
	{
        if(pos >= m_Length)
			return false;

        char * pPos = m_String + pos + 1;
        // while pointer pPos points correctly to m_String buffer
		//						and 
		//		 the parameter in buffer has the same type as parameter array pParams
        while(((pPos - m_String) < static_cast<int>(m_Length) ) && (*pPos == getType(pParams)))
		{
			// copy parameter to parameter array
			memcpy(pParams + paramsCnt, ++pPos, sizeof(T));
			// increment parameter counter
			paramsCnt++;
            // move pointer behind the parameter in m_String buffer
			pPos += sizeof(T)+1;
		}
        pos = pPos - m_String - 1;
		return true;
	}

    /**
      * Get pointer to string data in LongString in raw form
      * @return all raw data of LongString
      */
	char * getData( );


    /**
      * Get pointer to string data in LongString in raw form
      * @param pos position of first byte in the string to be taken into consideration
      * @param length maximum length of taken data
      * @param delimiter optional delimiter
      * @return raw data
      */
    char * getData( unsigned int & pos, unsigned int & length, char delimiter);

    /**
      * Returns data whole symbol/module with it's parameters
      * @param pos Position of module. Will be changet to position after module
      * @return module in raw data form
      */
    char * getSymbol( unsigned int & pos );

    /**
      * Matches right context
      * @param context right context
      * @param pos initial position - might be right behind identifier character
      * @param ignore string of characters that should bee ignored during matching algorithm
      * @return position of matching context or -1 if not found
      */
    int matchRight( char context, int pos, const std::string * ignore = NULL, const std::string * = NULL );

    /**
      * Matches left context
      * @param context right context
      * @param pos initial position - might be right behind identifier character
      * @param ignore string of characters that should bee ignored during matching algorithm
      * @return position of matching context or -1 if not found
      */
	int matchLeft( char, int, const std::string * = NULL, const std::string * = NULL );

    /**
      * Peek neighbouring symbol's identifier.
      * @param pos initial position
      * @param direction true for searching to the right; false for searching to the left
      * @return module's/symbol's identifier
      */
    char peekSymbol( int & pos, bool direction );

    /**
      * Look for matching right bracket
      * @param pos initial position
      * @return position of matching bracket.
      */
    int findMatchingRightBracket( int pos );
    int findMatchingLeftBracket( int pos );
    
    /**
      * Debug method for converting LongString from internal raw format to standart string.
      * Becareful - very slow !
      * @return string in standart L-system format
      */
    std::string toString( );

    /**
      * Access to any byte of string
      */
    inline char& operator[](unsigned int i) const
    {
        return m_String[i];
    }

    /**
      * Length of longstring
      * @return length
      */
    inline unsigned int length() const
    {
        return m_Length;
    }
};
}
#endif // LONGSTRING_H
