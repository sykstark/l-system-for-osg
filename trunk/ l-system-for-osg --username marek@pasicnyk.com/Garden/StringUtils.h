#pragma once
#include "AbstractGrammar.h"
#include "Configuration.h"

namespace AP_LSystem {

class StringUtils
{
public:	
	static void uncommentLine(string &);
	static std::string processLine(std::fstream * , std::stringstream & );
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
	};
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
		int cnt = sprintf( pStr + _length, "%.3f", i );
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
	void getParamaters( unsigned int & pos, double * pParams, int & paramsCnt )
	{
		if( (pos + 1 > _length) || ( pStr[pos + 1] != '(' ) )
			return;
		char * pPos = pStr + pos + 2; // skip nonterminal and (
		char * pEnd = 0;
		do
		{
			pParams[paramsCnt++] = strtod( pPos, &pEnd );
			pPos = pEnd + 1;
		}
		while ( *pEnd == ',' );

		pos = pPos - pStr;
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


	// zkusit vytvorit appendChar()
};
}