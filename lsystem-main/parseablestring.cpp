#include "precompiled.h"

#include "parseablestring.h"
#include <cstring>

using namespace AP_LSystem;

namespace AP_LSystem {
std::basic_ostream<char> & operator<<(std::basic_ostream<char> & os, const ParseableString ps)
{
    os.write( ps.m_Str, ps.m_Length );
    return os;
}
}

ParseableString::ParseableString( LongString * str): m_Pos(0),m_Eof(false),m_Length(str->length())
{
    m_Str = new char[m_Length];
    memcpy(m_Str, str->getData(), m_Length);
}

ParseableString::ParseableString( char * string, unsigned int length) : m_Pos(0),m_Eof(false), m_Length(length)
{
    m_Str = new char[length];
    memcpy(m_Str, string, length);
}

ParseableString::ParseableString(const ParseableString & c):m_Pos(c.m_Pos),m_Eof(c.m_Eof),m_Length(c.m_Length)
{
    m_Str = new char[m_Length];
    memcpy(m_Str, c.m_Str, c.m_Length);
}

char ParseableString::next(std::vector<Parameter> &parameters)
{
    if( m_Eof )
        return '\0';

    char symbol = m_Str[m_Pos];

    if(m_Pos++ >= m_Length)
    {
        m_Eof = true;
        return '\0';
    }

    while(true)
    {
        switch(m_Str[m_Pos])
        {
        case Parameter::LS_DOUBLE:
            {
                //double * pPar = reinterpret_cast<double *>(m_Str);
                parameters.push_back( Parameter( static_cast<void *>(m_Str + m_Pos + 1), Parameter::LS_DOUBLE ) );
                m_Pos += sizeof(double)+2;
            }
            break;
        case Parameter::LS_UBYTE:
            {
                parameters.push_back( Parameter( static_cast<void *>(m_Str + m_Pos + 1), Parameter::LS_UBYTE ) );
                m_Pos += sizeof(unsigned char)+2;
            }
            break;
        default:
            if(m_Pos >= m_Length)
                m_Eof = true;
            return symbol;
        }
    }
}
