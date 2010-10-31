#include "precompiled.h"

#include "rectangle.h"

using namespace AP_LSystem;

int Rectangle::drawStep(double dist)
{
	// get vertex and normal arrays for pipe geometry
	osg::Vec3dArray * v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
	osg::Vec3dArray * n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
	osg::Vec2dArray * t = dynamic_cast<osg::Vec2dArray*>(properties.geometry->getTexCoordArray(0));
	unsigned int first = v->size();

	v->push_back( LeftVec * properties.radius * properties.matrix );
	v->push_back( -LeftVec * properties.radius * properties.matrix );

	properties.matrix.preMultTranslate( HeadVec * dist );
	
	v->push_back( -LeftVec * properties.radius * properties.matrix );
	v->push_back( LeftVec * properties.radius * properties.matrix );

	// add normals - orthogonal to rectangle
	for( int i = 0; i < 4; i++ )
		n->push_back( properties.matrix.getRotate() * UpVec );

	// add texture coordinates
	t->push_back( osg::Vec2d( 0.0f, 0.0f ) );
	t->push_back( osg::Vec2d( 1.0f, 0.0f ) );
	t->push_back( osg::Vec2d( 1.0f, 1.0f ) );
	t->push_back( osg::Vec2d( 0.0f, 1.0f ) );

	// add rectangle primitive
	properties.geometry->addPrimitiveSet( 
		new osg::DrawArrays(osg::PrimitiveSet::QUADS, first, 4 ) );

	return LS_OK;
}