#include "precompiled.h"

#include "straightpipe.h"

using namespace AP_LSystem;

int StraightPipe::insideStep( )
{
	osg::ref_ptr<osg::Vec3dArray> contourV = new osg::Vec3dArray;
	osg::ref_ptr<osg::Vec3dArray> contourN = new osg::Vec3dArray;

	// get vertex and normal arrays for pipe geometry
	osg::Vec3dArray * v = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getVertexArray());
	osg::Vec3dArray * n = dynamic_cast<osg::Vec3dArray*>(properties.geometry->getNormalArray());
	osg::Vec2dArray * t = dynamic_cast<osg::Vec2dArray*>(properties.geometry->getTexCoordArray(0));
	unsigned int first = v->size();

	osg::Vec3dArray::iterator lastV;
	osg::Vec3dArray::iterator lastN;
	osg::Vec3dArray::iterator cont;

	osg::Matrixd contMat = properties.matrix;
	contMat(0,2) = properties.contourVec.x();
	contMat(1,2) = properties.contourVec.y();
	contMat(2,2) = properties.contourVec.z();

	if( properties.flags & TurtleProperties::MINIMIZE_TWIST )
	{
		osg::Vec3d h = contMat * HeadVec;
		h = h^properties.contourVec;
		h.normalize();

		contMat(0,0) = h.x();
		contMat(1,0) = h.y();
		contMat(2,0) = h.z();

		drawVector( properties.contourVec, properties.matrix, osg::Vec4d( 1.0,0.0,1.0,1.0 ) );

		drawVector( LeftVec, contMat, osg::Vec4d( 1.0,1.0,0.0,1.0 ) );

		//bool val = contMat.valid();
	}
	

	//double texCoordInc = properties.texCoordS + 
	double texSInc = properties.texRepeatingS / static_cast<double>(properties.contourDetail);
	double texTInc = (Center*properties.matrix - properties.controlPoint).length();
	double texS = 0;

	// adjust texture coordinates in T direction to S direction to preserve the aspect ratio
	texTInc *= static_cast<double>(properties.texRepeatingS) / (2.0f * PI * properties.radius); 

	// create a pipe between last and current contour
	for( lastV = properties.contourLastV->begin(), lastN = properties.contourLastN->begin(),
		cont = properties.contour->begin(); cont != properties.contour->end(); lastV++, lastN++, cont++, texS += texSInc )
	{
		osg::Vec3d vec;
		// add vertex and normal of the current contour
		if( properties.flags & TurtleProperties::MINIMIZE_TWIST )
			vec = (*cont * properties.radius) * contMat;
		else
			vec = (*cont * properties.radius) * properties.matrix;

		v->push_back( vec );		
		n->push_back( properties.matrix.getRotate() * *cont  ); // TODO rotate
		t->push_back(osg::Vec2d( texS, properties.texCoordT + texTInc ) );

		// store for future use as previous contour
		contourV->push_back( vec );
		contourN->push_back( properties.matrix.getRotate() * *cont );

		// add vertex and normal of last contour
		v->push_back( *lastV );
		n->push_back( *lastN );
		t->push_back(osg::Vec2d( texS, properties.texCoordT ) );
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

	// pipe is created as triangle strip
	properties.geometry->addPrimitiveSet( 
		new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, first, properties.contourDetail * 2 + 2 ) );

	// set current contour as last for next step
	properties.contourLastV = contourV;
	properties.contourLastN = contourN;

	return LS_OK;
}

void StraightPipe::setProperties( TurtleProperties p )
{
	AbstractTurtle::setProperties( p );

	// TODO kdyz neni definovana jina kontura
	createCircleContour( );
}

int StraightPipe::preStep( )
{
	initializePipe( );

	return LS_OK;
}

void StraightPipe::initializePipe()
{
	// beginning of drawing must be initialized by creating an initializing contour

	// no drawing step before
	if( ! properties.contourLastV )
	{
		// create arrays for storing first contour
		properties.contourLastV = new osg::Vec3dArray;
		properties.contourLastN = new osg::Vec3dArray;

		// create first contour
		for(osg::Vec3dArray::iterator it = properties.contour->begin(); it != properties.contour->end(); it++ )
		{
			properties.contourLastV->push_back( (*it * properties.radius) * properties.matrix );
			properties.contourLastN->push_back( properties.matrix.getRotate() * *it );
		}
	}
}