#include "precompiled.h"

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

	/*osg::Matrixd mat = properties.matrix;
	mat(0,2) = properties.contourVec.x();
	mat(1,2) = properties.contourVec.y();
	mat(2,2) = properties.contourVec.z();

	osg::Vec3d h = mat * HeadVec;
	h = h^properties.contourVec;
	h.normalize();

	mat(0,0) = h.x();
	mat(1,0) = h.y();
	mat(2,0) = h.z();
*/
	/*drawVector( LeftVec, mat, osg::Vec4d( 1.0,1.0,0.0,1.0 ) );
	osg::Vec3d x = LeftVec * mat;
	osg::Sphere * twist = new osg::Sphere(x, properties.debugGeometryScale * 0.2f);
	osg::ShapeDrawable * shape = new osg::ShapeDrawable(twist);
	shape->setColor( osg::Vec4f( 0.0f, 1.0f, 1.0f, 1.0f));
	geode->addDrawable( shape );*/
}

int MovingTurtle::makeRotate(osg::Quat & q)
{
	preRotate();

	

	osg::Matrixd x = osg::Matrixd::rotate( q ) ;

	osg::Vec3d h1 = properties.matrix.getRotate( ) * HeadVec;

	// TODO test - rotating directly by quaternion
	properties.matrix =  x * properties.matrix;

	osg::Vec3d h2 = properties.matrix.getRotate( ) * HeadVec;

	if( h1 != h2 )
	{
		osg::Matrixd m;
		m.makeRotate( h1, h2 );
		properties.contourVec = properties.contourVec * m ; 
		properties.contourVec.normalize();

		
	}	
	postRotate();

	//drawDebugGeometry( );

	return LS_OK;
}

int MovingTurtle::drawStep( double dist)
{
	int ret = LS_NOTDEFINED;
	ret = preStep();

	if( !ret )
		properties.matrix = osg::Matrixd::translate( osg::Vec3d( 0.0, dist, 0.0 ) ) * properties.matrix;
	else
		return ret;

	drawFrame( properties.matrix );

	return postStep();
}

int MovingTurtle::doStep( double dist)
{
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