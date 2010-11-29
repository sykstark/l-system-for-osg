#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

namespace AP_LSystem {
class StringUtils
{
public:	
    static void uncommentLine(std::string &);
    static std::string processLine(std::fstream * , std::stringstream & );
	static int processVector( std::string str, std::vector<double> & v );
};



struct Parameter
{
    enum Type
    {
        LS_BYTE = 1,
        LS_UBYTE,
        LS_INT,
        LS_UINT,
        LS_FLOAT,
        LS_DOUBLE
    };

    Parameter( void * value, Type type ):value(value),type(type){}

	void * value;
    Type type;
};
}

#endif
