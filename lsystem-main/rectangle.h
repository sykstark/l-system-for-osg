#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "loftturtle.h"

namespace AP_LSystem {
class Rectangle : public MovingTurtle
{
private:
	
public:
	Rectangle(void){};
	~Rectangle(void){};

	virtual int drawStep( double );

//	virtual void setProperties( TurtleProperties p );
};
}

#endif