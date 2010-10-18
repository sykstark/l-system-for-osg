#ifndef PARSEABLESTRING_H_
#define PARSEABLESTRING_H_

#include <vector>
#include "utils.h"
#include "longstring.h"

namespace AP_LSystem {
class ParseableString
{
private:
	char * pStr;
	unsigned int _pos, _length;
	bool _eof;
public:
	ParseableString( LongString * string): pStr(string->getString()), _pos(0), _length(string->length()), _eof(false) { }
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
			switch(pStr[_pos])
			{
			case LS_DOUBLE:
				{
					//double * pPar = reinterpret_cast<double *>(pStr);
					parameters.push_back( Parameter( static_cast<void *>(pStr + _pos + 1), LS_DOUBLE ) );
                    _pos += sizeof(double)+2;
				}
				break;
            case LS_UBYTE:
                {
                    parameters.push_back( Parameter( static_cast<void *>(pStr + _pos + 1), LS_UBYTE ) );
                    _pos += sizeof(unsigned char)+2;
                }
                break;
			case LS_NO_PARAMETER:
				_pos++;
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
