#include "precompiled.h"
#include "jointedpipe.h"

using namespace AP_LSystem;

JointedPipe::JointedPipe(void)
{
	//createContour();
}

JointedPipe::~JointedPipe(void)
{
}

int JointedPipe::preStep()
{
	//geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( osg::Vec3d( 0.0,0.0,0.0), 1.0 ) ) );
	return 0;
}

int JointedPipe::postStep()
{
	return 0;
}

int JointedPipe::drawStep(double dist)
{
	osg::Matrixd translated = osg::Matrixd::translate(osg::Vec3d( 0.0, dist, 0.0 )) * properties.matrix;
	//drawDebugGeometry();

	osg::Vec3dArray * v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
	osg::Vec3dArray * n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
	unsigned int first = v->size();
	
	osg::Vec3dArray::iterator it;
	for(it = properties.contour->begin(); it != properties.contour->end(); it++ )
	{
		geode->addDrawable( new osg::ShapeDrawable( new osg::Sphere( *it, 0.3 ) ) );
		v->push_back( *it * properties.matrix );
		v->push_back( *it * translated );
		n->push_back( *it );
		n->push_back( *it );
	}

	it = properties.contour->begin();
	v->push_back( *it * properties.matrix );
	v->push_back( *it * translated );

	osg::Vec3d vn = *it;
	vn.normalize();
	n->push_back( vn );
	n->push_back( vn );

	properties.geometry->addPrimitiveSet( 
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, first, properties.contourDetail * 2 + 2 ) );

	properties.matrix = translated;

	return LS_OK;
}

void JointedPipe::setProperties( TurtleProperties p )
{
	AbstractTurtle::setProperties( p );

	createContour( );
}

void JointedPipe::createContour()
{
	if(properties.contour.get())
		return;

	osg::Quat q( 2.0f * PI / static_cast<double>(properties.contourDetail), HeadVec );

	properties.contour = new osg::Vec3dArray( );

	properties.contour->push_back( LeftVec * properties.radius );

	for(unsigned int i=1; i<properties.contourDetail; i++)
	{
		properties.contour->push_back( q * *(properties.contour->rbegin()) );		
	}
}

void JointedPipe::createHemisphere()
{

}