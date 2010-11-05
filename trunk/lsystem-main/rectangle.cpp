#include "precompiled.h"

#include "rectangle.h"

using namespace AP_LSystem;

int Rectangle::drawStep(double dist)
{
	osg::ref_ptr<osg::Vec3dArray> v;
	osg::ref_ptr<osg::Vec3dArray> n;
	osg::ref_ptr<osg::Vec2dArray> t;

	// get vertex and normal arrays for pipe geometry
	if( properties.flags & TurtleProperties::SEPARATE_GEOMETRY_FOR_TRANSLUCENT )
	{
		osg::ref_ptr<osg::Geometry> g = new osg::Geometry();
		v = new osg::Vec3dArray();
		n = new osg::Vec3dArray();
		t = new osg::Vec2dArray();
		osg::ref_ptr<osg::Vec4dArray> c = new osg::Vec4dArray;
		g->setColorArray( c.get() );
		g->setNormalArray( n.get() );
		g->setVertexArray( v.get() );
		g->setTexCoordArray( 0, t.get() );
		g->setNormalBinding( osg::Geometry::BIND_PER_VERTEX );
		g->setColorBinding( osg::Geometry::BIND_OVERALL );
		this->geode->addDrawable(g.get() );
		c->push_back( osg::Vec4( 1.0, 1.0, 1.0, 1.0 ) );

		// add rectangle primitive
		g->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4 ) );
	}
	else
	{
		v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
		n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
		t = dynamic_cast<osg::Vec2dArray*>(properties.geometry->getTexCoordArray(0));

		// add rectangle primitive
		properties.geometry->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::QUADS, v->size(), 4 ) );
	}

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

	

	

	return LS_OK;
}