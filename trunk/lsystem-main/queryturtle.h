#ifndef QUERYTURTLE_H_
#define QUERYTURTLE_H_

#include "movingturtle.h"

namespace AP_LSystem {
/**
 * Specialized turtle for query processing. Doesn't generate any geometry. Only for determination of position matrix.
 */
class QueryTurtle : public MovingTurtle
{	
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