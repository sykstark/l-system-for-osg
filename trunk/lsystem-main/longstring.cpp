#include "precompiled.h"

#include <cstdio>
#include "longstring.h"
#include "lsystemexception.h"
#include "log.h"
#include "boost/lexical_cast.hpp"

using boost::lexical_cast;
using boost::bad_lexical_cast;
using namespace AP_LSystem;

LongString::LongString( unsigned int increment): m_Length(0)
{
    m_Allocated = m_Increment = increment;
    m_String = new char[m_Allocated];
}

LongString::LongString( const LongString& c ):m_Length(c.m_Length),
    m_Allocated(c.m_Allocated), m_Increment( c.m_Increment)
{
    m_String = new char[m_Allocated];
    memcpy( m_String, c.m_String, c.m_Length );
}

LongString& LongString::operator=( const LongString & c )
{
    m_Length = c.m_Length;
    m_Allocated = c.m_Allocated;
    m_Increment = c.m_Increment;
    m_String = new char[m_Allocated];
    memcpy( m_String, c.m_String, c.m_Length );

    return *this;
}

LongString::~LongString( )
{
    if( m_String )
    {
        delete[] m_String;
    }
}

void LongString::resize()
{
    if(m_String)
    {
        char * pOld = m_String;

        m_Allocated += m_Increment;
        m_String = new char[m_Allocated];
        memcpy( m_String, pOld, m_Length );

        delete[] pOld;
    }
}

char LongString::peekSymbol( int & pos, bool forward )
{
    while( (forward)?( static_cast<unsigned>(pos) < m_Length ):( pos >= 0 ))
    {
        switch(m_String[pos])
        {
        case Parameter::LS_DOUBLE:
			pos += ((forward)?(1):(-1))*(sizeof(double)+2);
            break;
        case Parameter::LS_UBYTE:
            pos += ((forward)?(1):(-1))*(sizeof(unsigned char)+2);
            break;
        case Parameter::LS_INT:
			pos += ((forward)?(1):(-1))*(sizeof(int)+2);
			break;
        default:
            return m_String[pos];
        }
    }
	return '\0';
}

int LongString::findMatchingRightBracket( int pos )
{
	int count = 1; //pos++;
    while( (count) && (static_cast<unsigned>(++pos) < m_Length) )
	{
		peekSymbol( pos, true );
        switch( m_String[pos] )
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
        switch( m_String[pos] )
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
    if( static_cast<unsigned>(pos) >= m_Length )
	{
		return -1;
	}
	// left bracket found
    else if( m_String[pos] == '[' )
	{
		int i = matchRight( ch, pos + 1, ignore, consider );
		return ( i < 0 ) ? ( matchRight( ch, findMatchingRightBracket( pos ) + 1, ignore, consider ) ) : i;
	}
    else if( m_String[pos] == ']' )
	{
		return matchRight( ch, pos + 1, ignore, consider );
	}
	// if there is some chars in consider, ch must be inluded, otherwise match next
    else if( consider && (!consider->empty()) && (consider->find(m_String[pos]) == std::string::npos ) )
	{
		peekSymbol( pos, true );
		return matchRight( ch, pos, ignore, consider );
	}
    else if( ignore && ( ignore->find(m_String[pos]) != std::string::npos ) )
	{
		peekSymbol( ++pos, true );
		return matchRight( ch, pos, ignore, consider );
	}
    else if( m_String[pos] == ch )
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
    else if( m_String[pos] == '[' )
	{
		peekSymbol( pos, false );
		return matchLeft( ch, pos, ignore, consider );
	}
    else if( m_String[pos] == ']' )
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
    else if( m_String[pos] == ch )
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

void LongString::append( Parameter::Type type )
{
    m_String[m_Length++] = static_cast<unsigned char>(type);
}

void LongString::append( const char ch )
{
    if(m_Allocated < m_Length + 1)
    {
        resize( );
    }
    m_String[m_Length++] = ch;
}

void LongString::append(const char * buffer, int length)
{
    while(m_Allocated < m_Length + length)
    {
        resize( );
    }

    memcpy( m_String + m_Length, buffer, length );
    m_Length += length;
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
    if(pos >= m_Length)
        return NULL;
    char * pSymbol = m_String + pos;

	int i = pos + 1;
	// move pos to next symbol...
	peekSymbol( i, true );
	// ...and set pos as end of current symbol
	pos = i - 1;
	return pSymbol; 
}

char * LongString::getData( )
{
    return m_String;
}

char * LongString::getData( unsigned int & pos, unsigned int & length, char delimiter )
{
    if(pos >= m_Length)
        return NULL;
    char * pPos, * res;
    pPos = res = m_String + pos;
    while( pPos-m_String < static_cast<int>(m_Length) )
    {
        switch(*pPos)
        {          
        case Parameter::LS_DOUBLE:
            pPos += sizeof(double)+2;
            break;
        case Parameter::LS_UBYTE:
            pPos += sizeof(unsigned char)+2;
            break;
        case Parameter::LS_INT:
			pPos += sizeof(int)+2;
			break;
        default:
			if(*pPos == delimiter)
			{
				length = pPos - res;
                pos = pPos - m_String;
				return res;
			}
            pPos++;
        }
    }
	length = pPos - res;
    pos = pPos - m_String;
	return res; 
}

std::string LongString::toString( )
{
    std::string str;
   //str.resize(2 * m_Length);
    char number[50];
    double dblnum;
    unsigned char ubytenum;
    for(unsigned int i=0; i < m_Length; i++)
    {
        switch(m_String[i])
        {
        case Parameter::LS_DOUBLE:
            memcpy(&dblnum, m_String + i + 1, sizeof(double));
            sprintf( number, "%.2f", dblnum );
            i += sizeof(double)+1;
            str.append(1,'(');
            str.append( number );
            str.append(1,')');
            break;
        case Parameter::LS_UBYTE:
            memcpy(&ubytenum, m_String + i + 1, sizeof(unsigned char));
            sprintf( number, "%d", ubytenum );
            i += sizeof(unsigned char)+1;
            str.append(1,'(');
            str.append( number );
            str.append(1,')');
            break;
        case Parameter::LS_INT:
            memcpy(&ubytenum, m_String + i + 1, sizeof(int));
            sprintf( number, "%d", ubytenum );
            i += sizeof(int)+1;
            str.append(1,'(');
            str.append( number );
            str.append(1,')');
            break;
        default:
            str.append(1,m_String[i]);
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
