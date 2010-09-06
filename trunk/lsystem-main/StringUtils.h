#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "AbstractGrammar.h"
#include "Configuration.h"

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

	inline bool reset()
	{
		_pos = 0;
		_eof = false;
	}
/*	char next(double * pParams, int & paramsCnt) inline
	{
		if( _eof ) 
			return '\0';

		_pos++;

		if( (_pos + 1 >= _length) || ( pStr[pos + 1] != '(' ) )
            return false;
		char * pPos = pStr + pos + 2; // skip nonterminal and (
		char * pEnd = 0;
		do
		{
			pParams[paramsCnt++] = strtod( pPos, &pEnd );
			pPos = pEnd + 1;
		}
		while ( *pEnd == ',' );

		if(_pos + 1 >= _length)
			_eof = true;

		return pStr[_pos];
	}*/

	char next( vector<Parameter> &parameters )
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
					_pos += sizeof(double);
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
    LongString(): _length(0)
    {
        _allocated =_increment = 100000;
        pStr = new unsigned char[_allocated];
    }

    LongString( unsigned int increment )
    {
        _allocated =_increment = increment;
        pStr = new char[_allocated];
    }

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

	void appendDouble( double * par )
	{
		if(_allocated < _length + sizeof(double) + 2)
		{
			resize( );
		}
		appendType( LS_DOUBLE );
		memcpy( pStr + _length, par, sizeof(double) );
		length += sizeof(double);
		appendType( LS_DOUBLE );
	}

	char& operator[](unsigned int i) const
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
		if( (pos + 1 >= _length) || ( pStr[pos + 1] != '(' ) )
            return false;
		char * pPos = pStr + pos + 2; // skip nonterminal and (
		char * pEnd = 0;
		do
		{
			pParams[paramsCnt++] = strtod( pPos, &pEnd );
			pPos = pEnd + 1;
		}
		while ( *pEnd == ',' );

        // set position on ')'
        pos = pPos - pStr - 1;

        return true;
	}

	char * c_str( )
	{
		if(_allocated < _length + 1)
		{
			resize( );
		}
		pStr[_length] = '\0';

		return pStr;
	}

    unsigned int getAllocated() const { return _allocated; }
    unsigned int getIncrement() const { return _increment; }
    char * getString() const { return pStr; }

	// zkusit vytvorit appendChar()
};
}