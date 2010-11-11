#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

//using namespace std;

namespace AP_LSystem {
class StringUtils
{
public:	
    static void uncommentLine(std::string &);
    static std::string processLine(std::fstream * , std::stringstream & );
	static int processVector( std::string str, std::vector<double> & v );
};

enum ParameterType
{
    LS_NO_PARAMETER = 1,
    LS_BYTE,
    LS_UBYTE,
    LS_INT,
    LS_UINT,
    LS_FLOAT,
    LS_DOUBLE
};

struct Parameter
{
	Parameter( void * value, ParameterType type ):value(value),type(type){}

	void * value;
	ParameterType type;
};
}

#endif
