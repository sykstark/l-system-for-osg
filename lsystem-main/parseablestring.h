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
    /**
      * This constructor creates an instance by converting from LongString
      * @param string string for conversion
      */
    ParseableString( LongString * string);

    /**
      * This constructor creates an instance from a byte buffer
      * @param string buffer with string data
      * @param length length of buffer
      */
    ParseableString( char * string, unsigned int length);

    /**
      * Copy contructor
      */
    ParseableString( const ParseableString & );

    /**
      * Assignement operator
      */
    ParseableString & operator=( const ParseableString & );

    /**
      * Output operator
      */
    friend std::basic_ostream<char> & operator<<(std::basic_ostream<char> & os, const ParseableString ps);

    /**
      * Indicates end of string
      * @return true if end of string
      */
	inline bool eof()
	{
        return m_Eof;
	}

    /**
      * Reset position of iterator to begin of string
      */
    inline void reset()
	{
        m_Pos = 0;
        m_Eof = false;
	}

    /**
      * Move interator forward and return next module
      * @param parameters vector for storing module parameter
      * @return identifier of module
      */
    char next( std::vector<Parameter> & parameters );
};

std::basic_ostream<char> & operator<<(std::basic_ostream<char> & os, const ParseableString ps);
}
#endif
