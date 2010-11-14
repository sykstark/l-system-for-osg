#include "precompiled.h"

#include <cstdio>
#include "longstring.h"
#include "lsystemexception.h"
#include "log.h"
#include "boost/lexical_cast.hpp"

using boost::lexical_cast;
using boost::bad_lexical_cast;
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

char LongString::peekSymbol( int & pos, bool forward )
{
	while( (forward)?( static_cast<unsigned>(pos) < _length ):( pos >= 0 ))
    {
        switch(pStr[pos])
        {
        case LS_DOUBLE:
			pos += ((forward)?(1):(-1))*(sizeof(double)+2);
            break;
        case LS_UBYTE:
            pos += ((forward)?(1):(-1))*(sizeof(unsigned char)+2);
            break;
		case LS_INT:
			pos += ((forward)?(1):(-1))*(sizeof(int)+2);
			break;
        default:
            return pStr[pos];
        }
    }
	return '\0';
}

int LongString::findMatchingRightBracket( int pos )
{
	int count = 1; //pos++;
	while( (count) && (static_cast<unsigned>(++pos) < _length) )
	{
		peekSymbol( pos, true );
		switch( pStr[pos] )
		{
		case '[':
			count++;
			break;
		case ']':
			count--;
			break;
		// added for case of end of string
		// primarily for end of cut section during cut symbol processing
		case '$':
			return pos;
		}
	}
	return pos;
}

int LongString::findMatchingLeftBracket( int pos )
{
	int count = 1; pos--;
	while( (count) && (pos >= 0) )
	{
		peekSymbol( pos, false );
		switch( pStr[pos] )
		{
		case '[':
			count--;
			break;
		case ']':
			count++;
			break;
		}
	}
	return pos;
}

int LongString::matchRight( char ch, int pos, const std::string * ignore, const std::string * consider )
{
	// position is out of string
	if( static_cast<unsigned>(pos) >= _length )
	{
		return -1;
	}
	// left bracket found
	else if( pStr[pos] == '[' )
	{
		int i = matchRight( ch, pos + 1, ignore, consider );
		return ( i < 0 ) ? ( matchRight( ch, findMatchingRightBracket( pos ) + 1, ignore, consider ) ) : i;
	}
	else if( pStr[pos] == ']' )
	{
		return matchRight( ch, pos + 1, ignore, consider );
	}
	// if there is some chars in consider, ch must be inluded, otherwise match next
	else if( consider && (!consider->empty()) && (consider->find(pStr[pos]) == std::string::npos ) )
	{
		peekSymbol( pos, true );
		return matchRight( ch, pos, ignore, consider );
	}
	else if( ignore && ( ignore->find(pStr[pos]) != std::string::npos ) )
	{
		peekSymbol( ++pos, true );
		return matchRight( ch, pos, ignore, consider );
	}
	else if( pStr[pos] == ch )
	{
		return pos;
	}
	else
	{
		return -1;
	}
}

int LongString::matchLeft( char ch, int pos, const std::string * ignore, const std::string * consider )
{
	if( pos < 0 )
	{
		return -1;
	}
	// left bracket found
	else if( pStr[pos] == '[' )
	{
		peekSymbol( pos, false );
		return matchLeft( ch, pos, ignore, consider );
	}
	else if( pStr[pos] == ']' )
	{
		return matchLeft( ch, findMatchingLeftBracket(pos) - 1, ignore, consider );
	}
	// if there is some chars in consider, ch must be inluded, otherwise match next
	else if( consider && (!consider->empty()) && (consider->find(ch) == std::string::npos ) )
	{
		peekSymbol( pos, false );
		return matchLeft( ch, pos, ignore, consider );
	}
	else if( ignore && ( ignore->find(ch) != std::string::npos ) )
	{
		peekSymbol( pos, false );
		return matchRight( ch, pos, ignore, consider );
	}
	else if( pStr[pos] == ch )
	{
		return pos;
	}
	else
	{
		return -1;
	}
}

void LongString::convertFromString(std::string * source, unsigned int & pos, const char delimiter)
{
    double parDbl;
    unsigned int i;
    std::string::iterator end, begin = source->begin() + pos;

	std::string chars = "(){},";
    chars.append( 1, delimiter );

    // epsilon rule
    if( (source->size() > pos) && (source->at(pos)=='e') )
    {
        pos = source->find(delimiter, 0);
        return;
    }

	// test if we are inside brackets
	bool betweenBrackets;
	if( pos && (*(begin-1) == ',') )
		betweenBrackets = true;
	else
		betweenBrackets = false;
    while(true)
    {   
        i = source->find_first_of( chars, begin - source->begin( ) );
        if ( i == std::string::npos )
        {
            this->append(std::string(begin,source->end()));
            return;
        }
        end = source->begin() + i;
        std::string str(begin, end);

        //Log::write( str );

        switch(*end)
        {
		case ')':
			betweenBrackets = false;
			parDbl = lexical_cast<double>(str);
            this->append<double>(parDbl);
            break;
        case ',':
			betweenBrackets = true;
            parDbl = lexical_cast<double>(str);
            this->append<double>(parDbl);
            break;
		case '}':
			betweenBrackets = false;
			this->append(lexical_cast<int>(str));
            break;
        case '(':
		case '{':
			// already between brackets - this ( will be part of some expression
			if( betweenBrackets )
				throw bad_lexical_cast();
			betweenBrackets = true;
            this->append(str);
            break;
        default:
            if (*end == delimiter)
            {
                this->append(str);
                return;
            }
            throw ParsingException("String converting error");
        }
        begin = end+1;
        pos = i+1;
    }
}

void LongString::append( ParameterType type )
{
    pStr[_length++] = static_cast<unsigned char>(type);
}

void LongString::append( const char ch )
{
    if(_allocated < _length + 1)
    {
        resize( );
    }
    pStr[_length++] = ch;
}

void LongString::append(const char * buffer, int length)
{
    while(_allocated < _length + length)
    {
        resize( );
    }

    memcpy( pStr + _length, buffer, length );
    _length += length;
}

void LongString::append( std::string str )
{
	this->append( str.c_str(), str.length() );
}

void LongString::append( LongString * ls )
{
	this->append( ls->getData(), ls->length() );
}

char * LongString::getSymbol(unsigned int & pos)
{
    if(pos >= _length)
        return NULL;
    char * pSymbol = pStr + pos;

	int i = pos + 1;
	// move pos to next symbol...
	peekSymbol( i, true );
	// ...and set pos as end of current symbol
	pos = i - 1;
	return pSymbol; 
}

char * LongString::getData( )
{
    return pStr;
}

char * LongString::getData( unsigned int & pos, unsigned int & length, char delimiter )
{
	if(pos >= _length)
        return NULL;
    char * pPos, * res;
	pPos = res = pStr + pos;
    while( pPos-pStr < static_cast<int>(_length) )
    {
        switch(*pPos)
        {          
        case LS_DOUBLE:
            pPos += sizeof(double)+2;
            break;
        case LS_UBYTE:
            pPos += sizeof(unsigned char)+2;
            break;
		case LS_INT:
			pPos += sizeof(int)+2;
			break;
        default:
			if(*pPos == delimiter)
			{
				length = pPos - res;
				pos = pPos - pStr;
				return res;
			}
            pPos++;
        }
    }
	length = pPos - res;
	pos = pPos - pStr;
	return res; 
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