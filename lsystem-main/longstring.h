#ifndef LONGSTRING_H_
#define LONGSTRING_H_

#include "StringUtils.h"
#include <string>

namespace AP_LSystem {

static unsigned int ZERO = 0;

class LongString
{
private:
    unsigned char * pStr;
    unsigned int _length;
    unsigned int _allocated;
    unsigned int _increment;

    void resize();
    void appendType( ParameterType type );

public:
    LongString( unsigned int = 100000);
    LongString( const LongString& c );
    LongString& operator=( const LongString & c );
    ~LongString( );
    void convertFromString( std::string *, unsigned int & = ZERO, const char = '\0' );
    void appendStr( const char * str, unsigned int length );
    void appendStr( std::string str );
    void appendChar( const char ch, bool parametric );
    void appendDouble( double par );
    unsigned char& operator[](unsigned int i) const;
    unsigned int length() const;
    bool getParamaters( unsigned int & pos, double * pParams, int & paramsCnt );
    char * c_str( );
    std::string toString( );

    unsigned int getAllocated() const { return _allocated; }
    unsigned int getIncrement() const { return _increment; }
    unsigned char * getString() const { return pStr; }

    // zkusit vytvorit appendChar()
};
}
#endif // LONGSTRING_H
