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

	inline ParameterType getType(double) { return LS_DOUBLE; }
	inline ParameterType getType(double *) { return LS_DOUBLE; }
	inline ParameterType getType(unsigned int) { return LS_UINT; }
	inline ParameterType getType(unsigned char) { return LS_UBYTE; }
	inline ParameterType getType(int) { return LS_INT; }
	inline ParameterType getType(int *) { return LS_INT; }

    void resize();
    void appendType( ParameterType type );

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
	void append( std::string );
	void append( ParameterType );
	void append( const char *, int );
	void append( LongString * );

	template< class T >
	bool getParameters( unsigned int & pos, T * pParams, int & paramsCnt )
	{
		if(pos + 1 >= _length)
			return false;

		paramsCnt = 0;

		char * pPos = pStr + pos + 1;
		while(*pPos == getType(pParams))
		{
			memcpy(pParams + paramsCnt, ++pPos, sizeof(T));
			paramsCnt++;
			pPos += sizeof(T)+1;
		}
		if( paramsCnt == 0 )
			return false;
		pos = pPos - pStr - 1;
		return true;
	}

	char * getData( );
	char * getData( unsigned int &, unsigned int &, char);
	char * getSymbol( unsigned int & pos);
	

/*    void appendStr( const char * str, unsigned int length );
    void appendStr( std::string str );
//    void appendStr( StaticString & str);
    void appendChar( const char ch, bool parametric );
    void appendDouble( double par );
	void appendInt( int par );
    void appendUByte( unsigned char par );
    void appendData( char *, int);*/
    
 //   bool getParamaters( unsigned int & pos, double * pParams, int & paramsCnt );
    
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
