#include "precompiled.h"
#include "jointedpipe.h"

using namespace AP_LSystem;

JointedPipe::JointedPipe(void)
{
	createContour();
}

JointedPipe::~JointedPipe(void)
{
}

int JointedPipe::preStep()
{
	//geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( osg::Vec3d( 0.0,0.0,0.0), 1.0 ) ) );
	return 0;
}

void JointedPipe::createContour()
{
	if(properties.contour.get())
		return;

	osg::Quat q( 2.0f /** osg::PI*/ / static_cast<double>(properties.contourDetail), osg::Vec3d( 1.0, 0.0, 0.0) );

	properties.contour = new osg::Vec3dArray( );

	properties.contour->push_back( LeftVec * properties.radius );

	//for(unsigned int i=1; i<properties.contourDetail; i++)
	{
	//	properties.contour->push_back( q * *(properties.contour->end()-1) );		
	}
}