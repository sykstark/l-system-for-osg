#ifndef LOG_H
#define LOG_H

#include <string>
#include <sstream>
#include "windows.h"

namespace AP_LSystem {
class Log
{
public:
    //Log();

    static void write( std::string & str)
    {
        OutputDebugStringA( str.c_str() );
    }

    static void write( int i )
    {
        std::stringstream str;
        str << i;
        OutputDebugStringA( str.str().c_str() );
    }
};
}

#endif // LOG_H
