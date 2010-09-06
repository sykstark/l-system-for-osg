#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "AbstractGrammar.h"
#include "Configuration.h"

namespace AP_LSystem {

class StringUtils
{
public:	
    static void uncommentLine(std::string &);
    static std::string processLine(std::fstream * , std::stringstream & );
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
  /*  inline char next(double * pParams, int & paramsCnt)
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
};

class LongString
{
private:
    char * pStr;
    unsigned int _length;
    unsigned int _allocated;
    unsigned int _increment;

    void resize()
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

public:
    LongString(): _length(0)
    {
        _allocated =_increment = 100000;
        pStr = new char[_allocated];
    }

    LongString( unsigned int increment )
    {
        _allocated =_increment = increment;
        pStr = new char[_allocated];
    }

    LongString( const LongString& c ):_length(c.length()),
        _allocated(c.getAllocated()), _increment( c.getIncrement())
    {
        pStr = new char[_allocated];
        memcpy( pStr, c.getString(), c.length() );
    }

    LongString& operator=( const LongString & c )
    {
        _length = c.length();
        _allocated = c.getAllocated();
        _increment = c.getIncrement();
        pStr = new char[_allocated];
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

        for(unsigned int i=0 ; i < length; i++)
        {
                pStr[_length + i ] = str[i];
        }

        _length += length;
    }

    void appendChar( const char ch )
    {
        if(_allocated < _length + 1)
        {
                resize( );
        }
        pStr[_length++] = ch;
    }

	void appendDouble( const double i )
	{
		if(_allocated < _length + 20)
		{
			resize( );
		}
        int cnt = sprintf( pStr + _length, "%.1f", i );
		if( cnt > 0 )
		{
			_length += cnt;
		}
		else
		{
			//chyba
		}
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
