//#pragma once

#ifndef STATICSTRING_H_
#define STATICSTRING_H_

#include <string>
#include <cstring>
#include "StringUtils.h"
#include "longstring.h"

namespace AP_LSystem {
class StaticString
{
public:
    char * str;
	int length;

	StaticString( const char * str, int length ):length(length)
	{
		this->str = new char[length];
		memcpy( (void *) this->str, str, length);
    }

    StaticString( LongString * str)
    {
        this->length = str->length();
        this->str = new char[length];
        memcpy( (void *) this->str, str->getString(), length);
    }

    StaticString( char ch ):length(1)
    {
        this->str = new char[1];
        this->str[0] = ch;
    }

    StaticString( std::string str )
	{
		this->length = str.length();
		this->str = new char[length];
		memcpy( (void *) this->str, str.c_str(), length);
    }

	StaticString( const StaticString & c )
	{
		this->length = c.length;
		this->str = new char[this->length];
		memcpy( (void *) this->str, c.str, this->length);
	}

	StaticString& operator=( const StaticString & c )
	{
		this->length = c.length;
		this->str = new char[this->length];
		memcpy( (void *) this->str, c.str, this->length);

		return *this;
	}

    std::string toString()
    {
        return std::string(this->str, this->length);
    }

	~StaticString( )
	{
		if(str)
		{
			delete[] str;
		}
    }
};
}

#endif
