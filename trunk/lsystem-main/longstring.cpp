#include "longstring.h"

using namespace AP_LSystem;

LongString::LongString( unsigned int increment): _length(0)
{
    _allocated =_increment = increment;
    pStr = new unsigned char[_allocated];
}

LongString::LongString( const LongString& c ):_length(c.length()),
    _allocated(c.getAllocated()), _increment( c.getIncrement())
{
    pStr = new unsigned char[_allocated];
    memcpy( pStr, c.getString(), c.length() );
}

LongString& LongString::operator=( const LongString & c )
{
    _length = c.length();
    _allocated = c.getAllocated();
    _increment = c.getIncrement();
    pStr = new unsigned char[_allocated];
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
        unsigned char * pOld = pStr;

        _allocated += _increment;
        pStr = new unsigned char[_allocated];
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
    std::string::iterator end, begin = source->begin() + pos;
    while(true)
    {
        std::string chars = "(,)" + delimiter;
        pos = source->find_first_of( chars, begin - source->begin( ) );
        if ( pos == std::string::npos )
        {
            this->appendStr(std::string(begin,source->end()));
            return;
        }
        end = source->begin() + pos;
        std::string str(begin, end);

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

unsigned char& LongString::operator[](unsigned int i) const
{
    // mozna dodat kontrolu
    return pStr[i];
}

unsigned int LongString::length() const
{
    return _length;
}

//mozna nekdy bool
bool LongString::getParamaters( unsigned int & pos, double * pParams, int & paramsCnt )
{
    if(pos + 1 >= _length)
        return false;

    paramsCnt = 0;

    unsigned char * pPos = pStr + pos + 1;
    while(true)
    {
        switch(*pPos)
        {
        case LS_NO_PARAMETER:
            return false;
        case LS_DOUBLE:
            memcpy(pParams + paramsCnt, ++pPos, sizeof(double));
            pPos += sizeof(double)+2;
            break;
        default:
            return true;
        }
    }
}

char * LongString::c_str( )
{
    if(_allocated < _length + 1)
    {
        resize( );
    }
    pStr[_length] = '\0';

    return reinterpret_cast<char *>(pStr);
}

std::string LongString::toString( )
{
    std::string str;
    //str.resize(2 * _length);
    char number[50];
    double dblnum;
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
            str.append(1,'(');
            str.append( number );
            str.append(1,')');
            i += sizeof(double)+1;
            break;
        default:
            str.append(1,pStr[i]);
            break;
        }
    }

    return str;
}
