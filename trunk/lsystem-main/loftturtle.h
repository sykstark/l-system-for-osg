#ifndef LOFTTURTLE_H_
#define LOFTTURTLE_H_

#include "movingturtle.h"

namespace AP_LSystem {
class LoftTurtle : public MovingTurtle
{
protected:
	virtual int drawStep( double dist)
	{
		int ret = LS_OK;

		preStep( );

		osg::Matrixd t;
		// set movement as half of step
		t.makeTranslate( HeadVec * dist/2.0f );
		// move to the center of the step
		properties.matrix = t * properties.matrix;

		// adjust matrices - process tropism, twist minimalizing
		adjustMatrices( );
		
		// do whatever in the center
		ret = insideStep();
		// draw debug
		drawFrame( properties.matrix );

		// store control point - center of step
		properties.controlPoint = osg::Vec3d( 0.0, 0.0, 0.0 ) * properties.matrix;

		// move forward - second half of the step
		properties.matrix = t * properties.matrix;

		postStep( );

		return ret;
	}

	void createCircleContour()
	{
		if(properties.contour.get())
			return;

		osg::Quat q( 2.0f * PI / static_cast<double>(properties.contourDetail), HeadVec );

		properties.contour = new osg::Vec3dArray( );

		properties.contour->push_back( LeftVec );

		for(unsigned int i=1; i<properties.contourDetail; i++)
		{
			properties.contour->push_back( q * *(properties.contour->rbegin()) );		
		}
	}

	virtual int addContourLoftGeometry();

	virtual int createLoftGeometryOpening();	

	virtual int insideStep() { return 0; }
public:
	LoftTurtle(void);
	~LoftTurtle(void);
};
}

#endif
