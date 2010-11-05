#ifndef QUERYTURTLE_H_
#define QUERYTURTLE_H_

#include "movingturtle.h"

namespace AP_LSystem {
class QueryTurtle : public MovingTurtle
{
private:
	
	
public:
	QueryTurtle(void){};
	~QueryTurtle(void){};

	virtual int drawStep( double );

	osg::Vec3d getPosition();
	osg::Vec3d getHeading();
	osg::Vec3d getLeft();
	osg::Vec3d getUp();

//	virtual void setProperties( TurtleProperties p );

};
}


#endif