#include "precompiled.h"

#include <cmath>
#include "movingturtle.h"

using namespace AP_LSystem;

void MovingTurtle::drawFrame( osg::Matrixd & matrix, osg::Vec4d * color )
{
	if( ! (properties.flags & TurtleProperties::DRAW_DEBUG_GEOMETRY ) )
		return;

	drawVector( HeadVec, matrix, (color)?(*color):(osg::Vec4d(1.0,0.0,0.0,1.0)) );
	drawVector( LeftVec, matrix, (color)?(*color):(osg::Vec4d(0.0,1.0,0.0,1.0)) );
	drawVector( UpVec,   matrix, (color)?(*color):(osg::Vec4d(0.0,0.0,1.0,1.0)) );

	//geode->addDrawable( new osg::ShapeDrawable(new osg::Sphere(VecUtils::Vec3Transform(properties.matrix,osg::Vec3d(0.0f,0.0f,0.0f)), ratio * 0.2f ) ) );
}

void MovingTurtle::adjustMatrices( )
{
	// get current heading vector
	osg::Vec3d head = properties.matrix.getRotate() * HeadVec;
	
	// gravitropism
	// do gravitropism only with elasticity greater than this value - optimalization
	if( properties.gravitropismElasticity > 0.0001 )
	{
		osg::Vec3d gravityAxis = head ^ (-HeadVec );									// H x T
		double gravAngle = properties.gravitropismElasticity * gravityAxis.length();	// a = e * | H x T |
		gravityAxis.normalize();
		properties.matrix = osg::Matrixd::rotate(properties.matrix.getRotate()) *		// origin rotation
							osg::Matrixd::rotate( gravAngle, gravityAxis ) *			// add rotation for gravitropism
							osg::Matrixd::translate( properties.matrix.getTrans() );	// add origin translation
	}

	// tropism
	if( properties.tropismElasticity > 0.0001 )
	{
		osg::Vec3d tropismAxis = head ^ properties.tropismVector;						// H x T
		const double dotProd = head * properties.tropismVector;							// H . T
		const double tropAngle = properties.tropismElasticity * 
			( cos( properties.tropismAngle ) - ( sin( properties.tropismAngle ) * dotProd / tropismAxis.length() ) );
		tropismAxis.normalize();
		properties.matrix = osg::Matrixd::rotate(properties.matrix.getRotate()) *		// origin rotation
							osg::Matrixd::rotate( tropAngle, tropismAxis ) *			// add rotation for tropism
							osg::Matrixd::translate( properties.matrix.getTrans() );	// add origin translation
	}

	// frame for minimalizing twist
	osg::Vec3d headLast = properties.lastFrame.getRotate() * HeadVec;
	properties.lastFrame = properties.lastFrame * osg::Matrixd::rotate( headLast, head );

	drawFrame( properties.lastFrame * osg::Matrixd::translate( properties.matrix.getTrans() ), new osg::Vec4d( 1.0, 1.0, 0.5, 1.0 ) ); 
	//drawFrame( properties.lastFrame, new osg::Vec4d( 1.0, 1.0, 0.5, 1.0 ) ); 

	osg::Vec3d levy = properties.lastFrame * LeftVec;
	levy.normalize();
	//drawVector( head, osg::Matrixd::identity(), osg::Vec4d( 1.0,0.5,1.0, 1.0 ));
	
}

int MovingTurtle::makeRotate(osg::Quat & q)
{
	preRotate();

	// TODO test - rotating directly by quaternion
	properties.matrix =  osg::Matrixd::rotate( q ) * properties.matrix;

	postRotate();

	//drawDebugGeometry( );

	return LS_OK;
}

int MovingTurtle::drawStep( double dist)
{
	int ret = LS_NOTDEFINED;
	ret = preStep();

	if( !ret )
		return ret;
	
	properties.matrix = osg::Matrixd::translate( osg::Vec3d( 0.0, dist, 0.0 ) ) * properties.matrix;		

	drawFrame( properties.matrix );

	return postStep();
}

int MovingTurtle::doStep( double dist)
{
	if ( dist < 0 )
		return LS_ERR_DRAWFORWARD_NEGATIVEDISTANCE;
	properties.matrix = osg::Matrixd::translate( osg::Vec3d( 0.0, dist, 0.0 ) ) * properties.matrix;

	drawFrame( properties.matrix );

	return postStep();
}

int MovingTurtle::drawForward(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->drawStep( properties.length );
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->drawStep( *(static_cast<double *>(p[0].value)) );
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::moveForward(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->doStep( properties.length );
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->doStep( *(static_cast<double *>(p[0].value)) );
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::moveForwardHalf( )
{
	this->drawStep( properties.length / 2.0f );
	
	return LS_OK;
}

int MovingTurtle::turnLeft(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->makeRotate( osg::Quat( -properties.angle, osg::Vec3d(1.0,0.0,0.0) ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(1.0,0.0,0.0) ) ); 		
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}	

	return LS_OK;
}

int MovingTurtle::turnRight(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->makeRotate( osg::Quat( properties.angle, osg::Vec3d(1.0,0.0,0.0) ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( processAngle(*(static_cast<double *>(p[0].value))), osg::Vec3d(1.0,0.0,0.0) ) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::pitchDown(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->makeRotate( osg::Quat( properties.angle, LeftVec ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( processAngle(*(static_cast<double *>(p[0].value))), osg::Vec3d(0.0,0.0,1.0) ) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::pitchUp(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->makeRotate( osg::Quat( - properties.angle, LeftVec ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( - processAngle(*(static_cast<double *>(p[0].value))), osg::Vec3d(0.0,0.0,1.0) ) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	} 

	return LS_OK;
}

int MovingTurtle::rollLeft(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->makeRotate( osg::Quat( -properties.angle, osg::Vec3d(0.0,1.0,0.0) ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( - processAngle(*(static_cast<double *>(p[0].value))), osg::Vec3d(0.0,1.0,0.0) ) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::rollRight(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->makeRotate( osg::Quat( properties.angle, osg::Vec3d(0.0,1.0,0.0) ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( processAngle(*(static_cast<double *>(p[0].value))), osg::Vec3d(0.0,1.0,0.0) ) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::rollUntilHorizontal( )
{
	return LS_NOTDEFINED;
}

int MovingTurtle::tropism(std::vector<Parameter> & p)
{
	return LS_NOTDEFINED;
}

int MovingTurtle::rollArround( )
{
	return LS_NOTDEFINED;
}

int MovingTurtle::turnArround( )
{
	return LS_NOTDEFINED;
}

int MovingTurtle::randomTurnPitchRoll(std::vector<Parameter> & p)
{
	return LS_NOTDEFINED;
}

int MovingTurtle::multiplyLength(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->properties.length *= properties.lengthMultiplier;
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->properties.length *= *(static_cast<double *>(p[0].value) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::multiplyRadius(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->properties.length *= properties.radiusMultiplier;
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->properties.radius *= *(static_cast<double *>(p[0].value) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}

int MovingTurtle::multiplyAngle(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->properties.length *= properties.angleMultiplier;
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->properties.radius *= *(static_cast<double *>(p[0].value) ); 
		break;
	default:
		return LS_ERR_PAR_INVALIDCOUNT;
	}

	return LS_OK;
}