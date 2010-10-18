//#pragma once

#ifndef UTILS_H_
#define UTILS_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
//#include "lsystemexception.h"
//#include "AbstractGrammar.h"
//#include "Configuration.h"
#include <osg/vec3d>
#include <osg/matrixd>
#include <string>

//#include "boost/lexical_cast.hpp"

//using boost::lexical_cast;
using namespace std;

namespace AP_LSystem {
class StringUtils
{
public:	
    static void uncommentLine(std::string &);
    static std::string processLine(std::fstream * , std::stringstream & );
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

class VecUtils
{
public:
	static osg::Vec3d Vec3Transform( osg::Matrixd & m, osg::Vec3d & v)
	{
		osg::Vec4d u = m * osg::Vec4d( v, 1.0 );
		
		return osg::Vec3d( u.x()/u.w(), u.y()/u.w(), u.z()/u.w() );
	}
};
}
#endif
