#include "precompiled.h"

#include "longstring.h"
#include "lsystemexception.h"
//#include "windows.h"
#include "boost/lexical_cast.hpp"

using boost::lexical_cast;

using namespace AP_LSystem;

LongString::LongString( unsigned int increment): _length(0)
{
    _allocated =_increment = increment;
    pStr = new char[_allocated];
}

LongString::LongString( const LongString& c ):_length(c.length()),
    _allocated(c.getAllocated()), _increment( c.getIncrement())
{
    pStr = new char[_allocated];
    memcpy( pStr, c.getString(), c.length() );
}

LongString& LongString::operator=( const LongString & c )
{
    _length = c.length();
    _allocated = c.getAllocated();
    _increment = c.getIncrement();
    pStr = new char[_allocated];
    memcpy( pStr, c.getString(), c.length() );

    return *this;
}

LongString::~LongString( )
{
    if( pStr )
    {
        delete[] pStr;
    }
}

void LongString::resize()
{
    if(pStr)
    {
        char * pOld = pStr;

        _allocated += _increment;
        pStr = new char[_allocated];
        for( unsigned int i =0; i < _length; i++)
        {
                pStr[i] = pOld[i];
        }

        delete[] pOld;
    }
}

void LongString::convertFromString(std::string * source, unsigned int & pos, const char delimiter)
{
    double par;
    unsigned int i;
    std::string::iterator end, begin = source->begin() + pos;
    while(true)
    {
        std::string chars = "(,)" + delimiter;
        i = source->find_first_of( chars, begin - source->begin( ) );
        if ( i == std::string::npos )
        {
            this->appendStr(std::string(begin,source->end()));
            return;
        }
        end = source->begin() + i;
        std::string str(begin, end);

        OutputDebugStringA( str.c_str() );

        switch(*end)
        {
        case ',':
        case ')':
            par = lexical_cast<double>(str);
            this->appendDouble(par);
            break;
        case '(':
            this->appendStr(str);
            break;
        default:
            if (*end == delimiter)
            {
                this->appendStr(str);
                return;
            }
            throw ParsingException("String converting error");
        }
        begin = end+1;
        pos = i+1;
    }
}

void LongString::appendType( ParameterType type )
{
    pStr[_length++] = static_cast<unsigned char>(type);
}

void LongString::appendStr( const char * str, unsigned int length )
{
    while(_allocated < length + _length)
    {
        resize( );
    }

    memcpy( pStr + _length, str, length);

    _length += length;
}

/*void LongString::appendStr( StaticString & str)
{
    while(_allocated < str.length() + _length)
    {
        resize( );
    }
}*/

void LongString::appendStr( std::string str )
{
    while(_allocated < str.length() + _length)
    {
        resize( );
    }

    memcpy( pStr + _length, str.c_str(), str.length());

    _length += str.length();
}

void LongString::appendChar( const char ch, bool parametric )
{
    if(_allocated < _length + (parametric)?1:2)
    {
            resize( );
    }
    pStr[_length++] = ch;

    if(!parametric)
    {
        appendType( LS_NO_PARAMETER );
    }
}

void LongString::appendDouble( double par )
{
    if(_allocated < _length + sizeof(double) + 2)
    {
        resize( );
    }
    appendType( LS_DOUBLE );
    memcpy( pStr + _length, &par, sizeof(double) );
    _length += sizeof(double);
    appendType( LS_DOUBLE );
}

void LongString::appendUByte( unsigned char par )
{
    if(_allocated < _length + sizeof(unsigned char) + 2)
    {
        resize( );
    }
    appendType( LS_UBYTE );
    memcpy( pStr + _length, &par, sizeof(unsigned char) );
    _length += sizeof(unsigned char);
    appendType( LS_UBYTE );
}

void LongString::appendData(char * buffer, int length)
{
    if(_allocated < _length + length)
    {
        resize( );
    }

    memcpy( pStr + _length, buffer, length );
    _length += length;
}

char * LongString::getSymbol(unsigned int & pos)
{
    if(pos + 1 >= _length)
        return NULL;
    char * pSymbol = pStr + pos;
    char * pPos = pStr + pos + 1;
    while(true)
    {
        switch(*pPos)
        {
        case LS_NO_PARAMETER:
            pos = pPos - pStr + 1;
            return pSymbol;
        case LS_DOUBLE:
            pPos += sizeof(double)+1;
            break;
        case LS_UBYTE:
            pPos += sizeof(unsigned char)+1;
            break;
        default:
            pos = pPos - pStr;
            return pSymbol;
        }
    }
}

bool LongString::getParamaters( unsigned int & pos, double * pParams, int & paramsCnt )
{
    if(pos + 1 >= _length)
        return false;

    paramsCnt = 0;

    char * pPos = pStr + pos + 1;
    while(true)
    {
        switch(*pPos)
        {
        case LS_NO_PARAMETER:
            return false;
        case LS_DOUBLE:
            memcpy(pParams + paramsCnt, ++pPos, sizeof(double));
            paramsCnt++;
            pPos += sizeof(double)+1;
            break;
        case LS_UBYTE:
        /*    memcpy(pParams + paramsCnt, ++pPos, sizeof(double));
            paramsCnt++;
            pPos += sizeof(double)+1;
            break;*/
        default:
            pos = pPos - pStr - 1;
            return true;
        }
    }
}

char * LongString::get( )
{
    return pStr;
}

std::string LongString::toString( )
{
    std::string str;
    //str.resize(2 * _length);
    char number[50];
    double dblnum;
    unsigned char ubytenum;
    for(unsigned int i=0; i < _length; i++)
    {
        switch(pStr[i])
        {
        case LS_NO_PARAMETER:
            str.append("()");
            break;
        case LS_DOUBLE:
            memcpy(&dblnum, pStr + i + 1, sizeof(double));
            sprintf( number, "%.2f", dblnum );
            i += sizeof(double)+1;
            str.append(1,'(');
            str.append( number );
            str.append(1,')');
            break;
        case LS_UBYTE:
            memcpy(&ubytenum, pStr + i + 1, sizeof(unsigned char));
            sprintf( number, "%d", ubytenum );
            i += sizeof(unsigned char)+1;
            str.append(1,'(');
            str.append( number );
            str.append(1,')');
            break;
        default:
            str.append(1,pStr[i]);
            break;
        }
    }

    unsigned int i = 0;
    while( true )
    {
        i = str.find(")(", i);
        if ( i == std::string::npos )
            break;
        str.replace(i,2,",");
    }

    return str;
}
