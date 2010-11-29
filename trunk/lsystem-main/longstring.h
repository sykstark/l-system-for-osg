#ifndef LONGSTRING_H_
#define LONGSTRING_H_

#include <string>
#include "utils.h"

namespace AP_LSystem {
static unsigned int ZERO = 0;

class LongString
{
private:
    char * pStr;
    unsigned int _length;
    unsigned int _allocated;
    unsigned int _increment;

    inline Parameter::Type getType(double) { return Parameter::LS_DOUBLE; }
    inline Parameter::Type getType(double *) { return Parameter::LS_DOUBLE; }
    inline Parameter::Type getType(unsigned int) { return Parameter::LS_UINT; }
    inline Parameter::Type getType(unsigned char) { return Parameter::LS_UBYTE; }
    inline Parameter::Type getType(int) { return Parameter::LS_INT; }
    inline Parameter::Type getType(int *) { return Parameter::LS_INT; }

    void resize();
	void append( std::string );
    void append( Parameter::Type );

public:
    LongString( unsigned int = 1048576); // 2^20 bytes - 1 MB
    LongString( const LongString& c );
    LongString& operator=( const LongString & c );
    ~LongString( );
    
	void convertFromString( std::string *, unsigned int & = ZERO, const char = '\0' );
	
	template< class T >
	void append( T par )
	{
		if(_allocated < _length + sizeof( T ) + 2)
		{
			resize( );
		}
		append( getType(par) );
		memcpy( pStr + _length, &par, sizeof( T ) );
		_length += sizeof( T );
		append( getType(par) );
	}
	void append( const char ch );
	
	void append( const char *, int );
	void append( LongString * );

	template< class T >
	bool getParameters( unsigned int & pos, T * pParams, int & paramsCnt )
	{
		if(pos >= _length)
			return false;

		//paramsCnt = 0;

		char * pPos = pStr + pos + 1;
		// while pointer pPos points correctly to pStr buffer 
		//						and 
		//		 the parameter in buffer has the same type as parameter array pParams
		while(((pPos - pStr) < static_cast<int>(_length) ) && (*pPos == getType(pParams)))
		{
			// copy parameter to parameter array
			memcpy(pParams + paramsCnt, ++pPos, sizeof(T));
			// increment parameter counter
			paramsCnt++;
			// move pointer behind the parameter in pStr buffer
			pPos += sizeof(T)+1;
		}
		pos = pPos - pStr - 1;
		return true;
	}

	char * getData( );
	char * getData( unsigned int &, unsigned int &, char);
	char * getSymbol( unsigned int & );

	int matchRight( char, int, const std::string * = NULL, const std::string * = NULL );
	int matchLeft( char, int, const std::string * = NULL, const std::string * = NULL );
	char peekSymbol( int &, bool );
	int findMatchingRightBracket( int );
	int findMatchingLeftBracket( int );
    
    std::string toString( );

    inline char& operator[](unsigned int i) const
    {
        // mozna dodat kontrolu
        return pStr[i];
    }

    inline unsigned int length() const
    {
        return _length;
    }

    unsigned int getAllocated() const { return _allocated; }
    unsigned int getIncrement() const { return _increment; }
    char * getString() const { return pStr; }
};
}
#endif // LONGSTRING_H
