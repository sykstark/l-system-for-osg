#include "precompiled.h"

#include "straightpipe.h"

using namespace AP_LSystem;

int StraightPipe::insideStep( )
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
/*
void StraightPipe::setProperties( TurtleProperties p )
{
	AbstractTurtle::setProperties( p );

	// TODO kdyz neni definovana jina kontura
	createCircleContour( );
	initializePipe( );

}*/

int StraightPipe::initialize( )
{
	createCircleContour( );
	initializePipe( );
	
	return LS_OK;
}

int StraightPipe::finalize()
{
	finalizePipe();

	return LS_OK;
}

int StraightPipe::preStep( )
{
	//initializePipe( );

	return LS_OK;
}

void StraightPipe::initializePipe()
{
	// beginning of drawing must be initialized by creating an initializing contour

	// no drawing step before
	if( !properties.contourLastV || !properties.contourLastN )
	{
		createLoftGeometryOpening( );
	}
}

void StraightPipe::finalizePipe()
{
	// finalize pipe

	if( properties.contourLastV )
	{
		insideStep();
		drawCap();
	}
}

void StraightPipe::drawCap()
{
	// get vertex and normal arrays for pipe geometry
	osg::Vec3dArray * v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
	osg::Vec3dArray * n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
	osg::Vec2dArray * t = dynamic_cast<osg::Vec2dArray*>(properties.geometry->getTexCoordArray(0));

	// pipe is created as triangle strip
	properties.geometry->addPrimitiveSet( 
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, v->size(), properties.contourDetail + 2 ) );



	if( properties.flags & TurtleProperties::DRAW_PIPE_CAPS )
	{
		// TEXTURE S COORDINATE
		//compute how much will be texture increment of one part of contour ( of one side of n-side contour polygon )
		double texSInc = properties.texRepeatingS / static_cast<double>(properties.contourDetail);
		// initial S coordinate
		double texS = 0;

		osg::Vec3d normal = properties.matrix.getRotate( ) * HeadVec;
		v->push_back( Center * properties.matrix );
		n->push_back( normal ); 
		t->push_back(osg::Vec2d( properties.texRepeatingS / 2.0, 1.0 ) );

		for(osg::Vec3dArray::iterator itV = properties.contourLastV->begin(); 
			itV != properties.contourLastV->end(); itV++, texS += texSInc)
		{
			v->push_back( *itV );
			n->push_back( normal );
			t->push_back(osg::Vec2d( texS, properties.texCoordT ) );
		}

		v->push_back( *properties.contourLastV->begin() );
		n->push_back( normal );
		t->push_back(osg::Vec2d( texS, properties.texCoordT ) );
	}
}