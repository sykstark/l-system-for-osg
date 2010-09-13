//#pragma once

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "lsystemexception.h"
#include "AbstractGrammar.h"
#include "Configuration.h"

#include "boost/lexical_cast.hpp"

using boost::lexical_cast;

namespace AP_LSystem {

class StringUtils
{
public:	
    static void uncommentLine(std::string &);
    static std::string processLine(std::fstream * , std::stringstream & );
};

enum ParameterType
{
	LS_NO_PARAMETER = 1,
	LS_BYTE,
	LS_UBYTE,
	LS_INT,
	LS_UINT,
	LS_FLOAT,
	LS_DOUBLE
};

struct Parameter
{
	Parameter( void * value, ParameterType type ):value(value),type(type){}

	void * value;
	ParameterType type;
};

class ParseableString
{
private:
	char * pStr;
	unsigned int _pos, _length;
	bool _eof;
public:
	ParseableString( char * string ): pStr(string), _pos(0), _eof(false) 
	{
		_length = strlen( string );
    }

	inline bool eof()
	{
		return _eof;
	}

    inline void reset()
	{
		_pos = 0;
		_eof = false;
	}

    char next( std::vector<Parameter> &parameters )
	{
		if( _eof ) 
			return '\0';
		
		char symbol = pStr[_pos];

		if(_pos++ >= _length)
		{
			_eof = true;
			return '\0';
		}

		while(true)
		{
			switch(pStr[_pos++])
			{
			case LS_DOUBLE:
				{
					double * pPar = reinterpret_cast<double *>(pStr);
					parameters.push_back( Parameter( reinterpret_cast<void *>(pPar), LS_DOUBLE ) );
                    _pos += sizeof(double)+1;
				}
				break;
			case LS_NO_PARAMETER:
			default:
				if(_pos >= _length)
					_eof = true;
				return symbol;
			}
		}
	}
};

class LongString
{
private:
    unsigned char * pStr;
    unsigned int _length;
    unsigned int _allocated;
    unsigned int _increment;

    void resize()
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

	void appendType( ParameterType type )
	{
		pStr[_length++] = static_cast<unsigned char>(type);
	}

public:
/*    LongString(): _length(0)
    {
        _allocated =_increment = 100000;
        pStr = new unsigned char[_allocated];
    }*/

    LongString( unsigned int increment = 100000 ): _length(0)
    {
        _allocated =_increment = increment;
        pStr = new unsigned char[_allocated];
    }

/*    LongString( std::string & source, unsigned int pos = 0, const char delimiter='\0', unsigned int increment=100000)
    {
        double par;
        std::string::iterator end, begin = source.begin() + pos;
        while(true)
        {
            std::string chars = "(,)" + delimiter;
            pos = source.find_first_of( chars, begin - source.begin( ) );
            if ( pos == std::string::npos )
            {
                this->appendStr(string(begin,source.end()));
                return;
            }
            end = source.begin() + pos;
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
    }*/

    LongString( const LongString& c ):_length(c.length()),
        _allocated(c.getAllocated()), _increment( c.getIncrement())
    {
        pStr = new unsigned char[_allocated];
        memcpy( pStr, c.getString(), c.length() );
    }

    LongString& operator=( const LongString & c )
    {
        _length = c.length();
        _allocated = c.getAllocated();
        _increment = c.getIncrement();
        pStr = new unsigned char[_allocated];
        memcpy( pStr, c.getString(), c.length() );

        return *this;
    }

    ~LongString( )
    {
        if( pStr )
        {
            delete[] pStr;
        }
    }

    void appendStr( const char * str, unsigned int length )
    {
        while(_allocated < length + _length)
        {
            resize( );
        }

		memcpy( pStr, str, length);

        _length += length;
    }

    void appendStr( std::string str )
    {
        while(_allocated < str.length() + _length)
        {
            resize( );
        }

        memcpy( pStr, str.c_str(), str.length());

        _length += str.length();
    }

    void appendChar( const char ch, bool parametric )
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

    void appendDouble( double par )
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

    unsigned char& operator[](unsigned int i) const
	{
		// mozna dodat kontrolu
		return pStr[i];
	}

	unsigned int length() const
	{
		return _length;
	}

	//mozna nekdy bool
    bool getParamaters( unsigned int & pos, double * pParams, int & paramsCnt )
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

	char * c_str( )
	{
		if(_allocated < _length + 1)
		{
			resize( );
		}
		pStr[_length] = '\0';

        return reinterpret_cast<char *>(pStr);
	}

    unsigned int getAllocated() const { return _allocated; }
    unsigned int getIncrement() const { return _increment; }
    unsigned char * getString() const { return pStr; }

	// zkusit vytvorit appendChar()
};
}

#endif
