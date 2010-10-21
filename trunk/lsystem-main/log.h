#ifndef LOG_H_
#define LOG_H_

#include <string>
#include <sstream>
#include "windows.h"

namespace AP_LSystem {
class Log
{
private:
    Log(){}
    static Log * logInst;
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
       // if(!log) log = new Log();

        return logInst;
    }

    static void write( std::string str)
    {
 //       OutputDebugStringA( str.c_str() );
        //output( str.c_str() );
    }

    static void write( int i )
    {
 /*       std::stringstream str;
        str << i;
        OutputDebugStringA( str.str().c_str() );*/
    }
};
}

#endif // LOG_H
