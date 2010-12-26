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

	unsigned m_RewritingCounter;
	unsigned m_ModuleCounter;
	unsigned m_TurtleCounter;
	unsigned m_QueryCounter;

	Log(): m_RewritingCounter(0), m_ModuleCounter(0), m_TurtleCounter(0), m_QueryCounter(0) {}
    
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

	unsigned int getRewritingCounter() const {return m_RewritingCounter;}

	unsigned int getModuleCounter() const {return m_ModuleCounter;}

	unsigned int getTurtleCounter() const {return m_TurtleCounter;}

	unsigned int getQueryCounter() const {return m_QueryCounter;}

	void incRewritingCounter() { m_RewritingCounter++;}

	void incModuleCounter() { m_ModuleCounter++;}

	void incTurtleCounter() { m_TurtleCounter++;}

	void incQueryCounter() { m_QueryCounter++;}
};
}

#endif // LOG_H
