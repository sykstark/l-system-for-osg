#include "precompiled.h"

#include "hermitpipe.h"

using namespace AP_LSystem;

int HermitPipe::drawStep( double dist)
{
	int ret = LS_NOTIMPLEMENTED;

	preStep( );

	properties.curveDetail = 6;

	
	osg::Matrixd tmp, end, hermit, geomCond;
	end = osg::Matrixd::translate( HeadVec * dist/2.0f ) * properties.matrix;
	hermit.set( 2.0, -2.0, 1.0, 1.0, -3.0, 3.0, -2.0, -1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0 );

	osg::Vec3d P0 = Center * properties.lastMatrix;
	osg::Vec3d P1 = Center * end;
	osg::Vec3d p0 = properties.lastMatrix.getRotate() * HeadVec ;
	osg::Vec3d p1 = end.getRotate() * HeadVec ;
	osg::Vec4d pp0( p0.x(), p0.y(), p0.z(), 1.0 );
	osg::Vec4d pp1( p1.x(), p1.y(), p1.z(), 1.0 );
	//pp0.normalize();
	//pp1.normalize();
	pp1*= 2.5;
	pp0*= 2.5;
	drawVector( p0, properties.lastMatrix, osg::Vec4d( 0.0,1.0,1.0,1.0 ));
	drawVector( p0, end, osg::Vec4d( 0.0,1.0,0.0,1.0 ));
	geomCond.set( P0.x(),P0.y(),P0.z(),1.0,
				  P1.x(),P1.y(),P1.z(),1.0,
				  pp0.x(),pp0.y(),pp0.z(),pp0.w(),
				  pp1.x(),pp1.y(),pp1.z(),pp1.w() );
	hermit.postMult( geomCond );

	// set movement as half of step
	//t.makeTranslate( HeadVec * dist/2.0f );
	// move to the center of the step
	
	double t = 0;
	osg::Vec4d T;
	for( unsigned i = 1; i < properties.curveDetail; i++) 
	{
		t += 1.0/static_cast<double>(properties.curveDetail);
		T = osg::Vec4d( t*t*t, t*t, t, 1 );
		tmp.identity();
		T = T * hermit;
		tmp.setTrans( osg::Vec3d( T.x()/T.w(), T.y()/T.w(), T.z()/T.w() ) );
		drawFrame( tmp, new osg::Vec4d( 1.0,0.0,0.0,1.0 ) );
	}

	// adjust matrices - process tropism, twist minimalizing
	//adjustMatrices( );
	
	// do whatever in the center
	//ret = insideStep();
	// draw debug
	drawFrame( end );

	// store control point - center of step
	properties.controlPoint = osg::Vec3d( 0.0, 0.0, 0.0 ) * properties.matrix; // TODO remove
	properties.lastMatrix = end;

	// move forward - second half of the step
	properties.matrix = osg::Matrixd::translate( HeadVec * dist/2.0f ) * end;

	return ret;
}

int HermitPipe::initialize( )
{
	createCircleContour( );
	//initializePipe( );
	
	return LS_OK;
}

int HermitPipe::finalize()
{
	//finalizePipe();

	return LS_OK;
}

void HermitPipe::initializePipe()
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

void HermitPipe::finalizePipe()
{
	// finalize pipe

	if( properties.contourLastV )
	{
		insideStep();
		drawCap();
	}
}

void HermitPipe::drawCap()
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