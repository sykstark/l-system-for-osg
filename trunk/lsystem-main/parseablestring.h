#ifndef PARSEABLESTRING_H_
#define PARSEABLESTRING_H_

#include <vector>
#include "utils.h"
#include "longstring.h"
#include <iostream>

namespace AP_LSystem {
class ParseableString
{
private:
    unsigned m_Pos;
    bool m_Eof;
protected:
    char * m_Str;
    unsigned m_Length;
public:
    ParseableString( LongString * string): m_Pos(0), m_Eof(false),m_Str(string->getString()),m_Length(string->length()) { }
    ParseableString( char * string, unsigned int length): m_Pos(0), m_Eof(false),m_Str(string),m_Length(length) { }

    friend std::basic_ostream<char> & operator<<(std::basic_ostream<char> & os, const ParseableString ps);

	inline bool eof()
	{
        return m_Eof;
	}

    inline void reset()
	{
        m_Pos = 0;
        m_Eof = false;
	}

    char next( std::vector<Parameter> & );
};

std::basic_ostream<char> & operator<<(std::basic_ostream<char> & os, const ParseableString ps);
}
#endif
