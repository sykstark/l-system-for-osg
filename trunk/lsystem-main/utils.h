#ifndef UTILS_H_
#define UTILS_H_

#include <string>
#include <vector>

namespace AP_LSystem {
/**
  * Library class with some utility functions for strings
  */
class StringUtils
{
public:
    /**
      * Erases every char behind '//' including this sign
      */
    static void uncommentLine(std::string &);

    /**
      * Erases all white spaces in string
      */
    static std::string & eraseWhiteSpaces(std::string &);

    /**
      * Get one line from file and extract first word
      */
    static std::string processLine(std::fstream * , std::stringstream & );

    /**
      * Convert vector in string format ( double, double, ... , double ) to std::vector
      */
	static int processVector( std::string str, std::vector<double> & v );
};

/**
  * Structure for storing parameter values
  */
struct Parameter
{
    /**
      * Parameter types
      */
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

    void * value;   ///< value of parameter
    Type type;      ///< parameter type
};
}

#endif
