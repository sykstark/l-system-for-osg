#ifndef LS_LOG_H_
#define LS_LOG_H_

#include <string>
#include <sstream>
#include "windows.h"

namespace AP_LSystem {
class Log
{
private:
	static Log * logInst;
    Log(){}
    
    ~Log(){}

    enum Output
    {
        OUTPUTDEBUGSTRING,
        FILE,
        VRECKO,
    } type;

    void output( const char * str )
    {
        switch(type)
        {
        case OUTPUTDEBUGSTRING:
            OutputDebugStringA( str );
            break;
        case FILE:
            break;
        case VRECKO:
            break;
        }
    }

public: 

    static Log * get()
	{
		if(!logInst) logInst = new Log();

		return logInst;
	}

    void write( std::string str)
    {
        OutputDebugStringA( str.c_str() );
		output( str.c_str() );
    }

    void write( int i )
    {
        std::stringstream str;
        str << i;
        OutputDebugStringA( str.str().c_str() );
    }
};
}

#endif // LOG_H
