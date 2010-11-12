#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "movingturtle.h"

namespace AP_LSystem {
class Rectangle : public MovingTurtle
{
private:
	
public:
	Rectangle(void){};
	~Rectangle(void){};

	virtual int drawStep( double );

	virtual int initialize( ) {return LS_OK;}
	virtual int finalize( ) {return LS_OK;}

//	virtual void setProperties( TurtleProperties p );
};
}

#endif