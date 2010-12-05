#include "precompiled.h"
#include "loftturtle.h"

using namespace AP_LSystem;

LoftTurtle::LoftTurtle(void)
{
}

LoftTurtle::~LoftTurtle(void)
{
}

int LoftTurtle::addContourLoftGeometry()
{
	// get vertex and normal arrays for pipe geometry
	osg::Vec3dArray * v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
	osg::Vec3dArray * n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
	osg::Vec2dArray * t = dynamic_cast<osg::Vec2dArray*>(properties.geometry->getTexCoordArray(0));

	// pipe is created as triangle strip
	properties.geometry->addPrimitiveSet( 
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, v->size(), properties.contourDetail * 2 + 2 ) );

	// TEXTURE S COORDINATE
	//compute how much will be texture increment of one part of contour ( of one side of n-side contour polygon )
	double texSInc = properties.texRepeatingS / static_cast<double>(properties.contourDetail);
	// initial S coordinate
	double texS = 0;
	
	// TEXTURE T COORDINATE 
	// compute distance between two control points of pipe
	double texTInc = (Center*properties.matrix - properties.controlPoint).length();
	// adjust texture coordinates in T direction to S direction to preserve the aspect ratio
	texTInc *= static_cast<double>(properties.texRepeatingS) / (2.0f * PI * properties.radius); 

	osg::Vec3dArray::iterator lastV, lastN, cont;
	osg::Vec3d currentV, currentN;
	osg::ref_ptr<osg::Vec3dArray> contourV = new osg::Vec3dArray;
	osg::ref_ptr<osg::Vec3dArray> contourN = new osg::Vec3dArray;

	// create a pipe between last and current contour
	for( lastV = properties.contourLastV->begin(), lastN = properties.contourLastN->begin(),
		cont = properties.contour->begin(); cont != properties.contour->end(); lastV++, lastN++, cont++, texS += texSInc )
	{	
		// set vertex and normal coordinates of contour of current control point
		if( properties.flags & TurtleProperties::MINIMIZE_TWIST )
		{
			currentV = (*cont * properties.radius) * properties.lastFrame * osg::Matrixd::translate(properties.matrix.getTrans());
			currentN = properties.lastFrame.getRotate() * *cont;
		}
		else
		{
			currentV = (*cont * properties.radius) * properties.matrix;
			currentN = properties.matrix.getRotate() * *cont;
		}

		// add vertex, normal and texture coordinates of contour of current control point
		v->push_back( currentV );	
		n->push_back( currentN );
		t->push_back(osg::Vec2d( texS, properties.texCoordT + texTInc ) );
		
		// add vertex, normal and texture coordinates of contour of previous control point
		v->push_back( *lastV );
		n->push_back( *lastN );
		t->push_back(osg::Vec2d( texS, properties.texCoordT ) );

		// store for for next control point ( as previous contour )
		contourV->push_back( currentV );
		contourN->push_back( currentN );
	}
	
	// add first vertices for complete circle
	v->push_back( *contourV->begin() );
	n->push_back( *contourN->begin() );
	t->push_back(osg::Vec2d( properties.texRepeatingS, properties.texCoordT + texTInc ) );
	v->push_back( *properties.contourLastV->begin() );
	n->push_back( *properties.contourLastN->begin() );
	t->push_back(osg::Vec2d( properties.texRepeatingS, properties.texCoordT ) );

	// update texture coordinates in T direction - for next step
	properties.texCoordT += texTInc;
	// set texture coordinate to interval <0,1>
	properties.texCoordT -= static_cast<int>(properties.texCoordT);

	// set current contour as last for next step
	properties.contourLastV = contourV;
	properties.contourLastN = contourN;

	return LS_OK;
}

int LoftTurtle::createLoftGeometryOpening()
{
	// create arrays for storing opening contour
	properties.contourLastV = new osg::Vec3dArray;
	properties.contourLastN = new osg::Vec3dArray;

	// create first contour
	for(osg::Vec3dArray::iterator it = properties.contour->begin(); it != properties.contour->end(); it++ )
	{
		properties.contourLastV->push_back( (*it * properties.radius) * properties.matrix );
		properties.contourLastN->push_back( properties.matrix.getRotate() * *it );
	}
	
	return LS_OK;
}
