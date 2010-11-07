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

	virtual int initialize( )
	{
		return LS_OK;
	}
	virtual int finalize( )
	{
		return LS_OK;
	}
};
}


#endif