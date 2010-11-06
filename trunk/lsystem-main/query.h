#ifndef QUERY_H_
#define QUERY_H_

#include "queryinterpret.h"

namespace AP_LSystem {
class Query
{
private:
	// dont create an instances
	Query(){};
public:
	static double positionX( const double * )
	{
		return QueryInterpret::get()->positionX();
	}

	static double positionY( const double * )
	{
		return QueryInterpret::get()->positionY();
	}

	static double positionZ( const double * )
	{
		return QueryInterpret::get()->positionY();
	}

};
}

#endif