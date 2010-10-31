#ifndef LOFTTURTLE_H_
#define LOFTTURTLE_H_

#include "movingturtle.h"

namespace AP_LSystem {
class LoftTurtle : public MovingTurtle
{
protected:
	virtual int drawStep( double dist)
	{
		int ret = LS_NOTDEFINED;

		preStep( );

		osg::Matrixd t;
		// set movement as half of step
		t.makeTranslate( HeadVec * dist/2.0f );
		// move to the center of the step
		properties.matrix = t * properties.matrix;

		//properties.matrix.orthoNormalize( )

		osg::Vec3d head = properties.matrix.getRotate() * HeadVec;
		head.normalize();
		osg::Vec3d headLast = properties.lastFrame.getRotate() * HeadVec;
		headLast.normalize();
		osg::Matrixd m = osg::Matrixd::rotate( headLast, head );
		osg::Matrixd m2 = osg::Matrixd::rotate( properties.angle, LeftVec );
		osg::Matrixd m3 = osg::Matrixd::rotate( -properties.angle, LeftVec );

		properties.lastFrame = properties.lastFrame * m;

		drawFrame( properties.lastFrame * osg::Matrixd::translate( properties.matrix.getTrans() ), new osg::Vec4d( 1.0, 1.0, 0.5, 1.0 ) ); 
		//drawFrame( properties.lastFrame, new osg::Vec4d( 1.0, 1.0, 0.5, 1.0 ) ); 

		osg::Vec3d levy = properties.lastFrame * LeftVec;
		levy.normalize();
		//drawVector( head, osg::Matrixd::identity(), osg::Vec4d( 1.0,0.5,1.0, 1.0 ));
		
		// do whatever in the center
		ret = insideStep();
		// draw debug
		drawFrame( properties.matrix );

		// store control point - center of step
		properties.controlPoint = osg::Vec3d( 0.0, 0.0, 0.0 ) * properties.matrix;

		// move forward - second half of the step
		properties.matrix = t * properties.matrix;

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

	virtual int insideStep() { return 0; }
public:
	LoftTurtle(void);
	~LoftTurtle(void);
};
}

#endif
