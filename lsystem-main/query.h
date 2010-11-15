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
		return QueryInterpret::get()->positionZ();
	}

	static double headingX( const double * )
	{
		return QueryInterpret::get()->headingX();
	}

	static double headingY( const double * )
	{
		return QueryInterpret::get()->headingY();
	}

	static double headingZ( const double * )
	{
		return QueryInterpret::get()->headingZ();
	}

	static double upX( const double * )
	{
		return QueryInterpret::get()->upX();
	}

	static double upY( const double * )
	{
		return QueryInterpret::get()->upY();
	}

	static double upZ( const double * )
	{
		return QueryInterpret::get()->upZ();
	}

	static double leftX( const double * )
	{
		return QueryInterpret::get()->leftX();
	}

	static double leftY( const double * )
	{
		return QueryInterpret::get()->leftY();
	}

	static double leftZ( const double * )
	{
		return QueryInterpret::get()->leftZ();
	}
};
}

#endif