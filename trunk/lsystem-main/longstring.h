#ifndef LONGSTRING_H_
#define LONGSTRING_H_

#include <string>
#include "StringUtils.h"

namespace AP_LSystem {

static unsigned int ZERO = 0;

class LongString
{
private:
    char * pStr;
    unsigned int _length;
    unsigned int _allocated;
    unsigned int _increment;

    void resize();
    void appendType( ParameterType type );

public:
    LongString( unsigned int = 1048576); // 2^20 bytes - 1 MB
    LongString( const LongString& c );
    LongString& operator=( const LongString & c );
    ~LongString( );
    void convertFromString( std::string *, unsigned int & = ZERO, const char = '\0' );
    void appendStr( const char * str, unsigned int length );
    void appendStr( std::string str );
//    void appendStr( StaticString & str);
    void appendChar( const char ch, bool parametric );
    void appendDouble( double par );
    void appendUByte( unsigned char par );
    void appendData( char *, int);
    char * getSymbol( unsigned int & pos);
    bool getParamaters( unsigned int & pos, double * pParams, int & paramsCnt );
    char * get( );
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
