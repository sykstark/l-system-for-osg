#ifndef QUERY_H_
#define QUERY_H_

#include "queryinterpret.h"

namespace AP_LSystem {
/**
  * This class is an adapter class between FunctionParser and QueryInterpret. It contains a static methods that
  * are added as custom functions to FunctionParser instances. They return coordinates of position matrix vectors.
  */
class Query
{
private:
	// dont create an instances
	Query(){};
public:
	/**
	  * Query for position x coordinate.
	  */
	static double positionX( const double * )
	{
		return QueryInterpret::get()->positionX();
	}

	/**
	  * Query for position y coordinate.
	  */
	static double positionY( const double * )
	{
		return QueryInterpret::get()->positionY();
	}

	/**
	  * Query for position z coordinate.
	  */
	static double positionZ( const double * )
	{
		return QueryInterpret::get()->positionZ();
	}

	/**
	  * Query for heading x coordinate.
	  */
	static double headingX( const double * )
	{
		return QueryInterpret::get()->headingX();
	}

	/**
	  * Query for heading y coordinate.
	  */
	static double headingY( const double * )
	{
		return QueryInterpret::get()->headingY();
	}

	/**
	  * Query for heading z coordinate.
	  */
	static double headingZ( const double * )
	{
		return QueryInterpret::get()->headingZ();
	}

	/**
	  * Query for up x coordinate.
	  */
	static double upX( const double * )
	{
		return QueryInterpret::get()->upX();
	}

	/**
	  * Query for up y coordinate.
	  */
	static double upY( const double * )
	{
		return QueryInterpret::get()->upY();
	}

	/**
	  * Query for up z coordinate.
	  */
	static double upZ( const double * )
	{
		return QueryInterpret::get()->upZ();
	}

	/**
	  * Query for left x coordinate.
	  */
	static double leftX( const double * )
	{
		return QueryInterpret::get()->leftX();
	}

	/**
	  * Query for left y coordinate.
	  */
	static double leftY( const double * )
	{
		return QueryInterpret::get()->leftY();
	}

	/**
	  * Query for left z coordinate.
	  */
	static double leftZ( const double * )
	{
		return QueryInterpret::get()->leftZ();
	}
};
}

#endif