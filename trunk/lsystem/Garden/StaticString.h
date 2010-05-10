#pragma once

namespace AP_LSystem {
class StaticString
{
public:
	const char * str;
	int length;

	StaticString( const char * str, int length ):length(length)
	{
		this->str = new char[length];
		memcpy( (void *) this->str, str, length);
		// strcpy( this->str, str);
	}

	StaticString( string str )
	{
		this->length = str.length();
		this->str = new char[length];
		memcpy( (void *) this->str, str.c_str(), length);
		// strcpy( this->str, str.c_str() );
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

	~StaticString( )
	{
		if(str)
		{
			delete[] str;
		}
	}
	//StaticString( const string str ){};
};
}