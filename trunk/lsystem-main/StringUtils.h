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
#include <string>

#include "boost/lexical_cast.hpp"

using boost::lexical_cast;
using namespace std;

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
    ParseableString( char * string, unsigned int length): pStr(string), _pos(0), _length(length), _eof(false) { }

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
					//double * pPar = reinterpret_cast<double *>(pStr);
					parameters.push_back( Parameter( static_cast<void *>(pStr), LS_DOUBLE ) );
                    _pos += sizeof(double)+1;
				}
				break;
            case LS_UBYTE:
                {
                    parameters.push_back( Parameter( static_cast<void *>(pStr), LS_UBYTE ) );
                    _pos += sizeof(unsigned char)+1;
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
}

#endif
