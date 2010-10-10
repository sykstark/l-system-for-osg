#ifndef LSGEODE_H_
#define LSGEODE_H_

#include "abstractturtle.h"

namespace AP_LSystem {
/**
 * Turtle types
 */
enum TurtleType
{
	LS_TURTLE_JOINTEDPIPE,
	LS_TURTLE_SRAIGHTPIPE,
	LS_TURTLE_HERMITPIPE,
	LS_TURTLE_OBJMODEL,
};

/**
 * OSG geometric node with added 
 *  - capability to configure node properties from Configuration
 *  - storing of turtle type that will be used to generate geometry
 */
class LSGeode :	public osg::Geode
{
private:
	TurtleType turtleType;
public:
	void setTurtleType(std::string &);
	TurtleType getTurtleType();

	LSGeode();
	void configure();
};
}

#endif