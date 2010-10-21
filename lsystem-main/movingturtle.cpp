#include "precompiled.h"

#include "movingturtle.h"

using namespace AP_LSystem;

void MovingTurtle::drawDebugGeometry( )
{
/*	if( ! (properties.flags & LS_FL_DEBUG ) )
		return;*/

	float ratio = 0.3;

	osg::Cylinder * headCyl = new osg::Cylinder(VecUtils::Vec3Transform(properties.matrix, osg::Vec3d(0.0f,ratio * 1.5f,0.0f)),ratio * 0.05f,ratio * 3.0f);
	osg::Cone * headCone = new osg::Cone(VecUtils::Vec3Transform(properties.matrix, osg::Vec3d(0.0f,ratio * 3.1f,0.0f)),ratio * 0.15f,ratio * 0.5f);
	osg::Cylinder * upCyl = new osg::Cylinder(VecUtils::Vec3Transform(properties.matrix, osg::Vec3d(ratio * 1.5f,0.0f,0.0f)),ratio * 0.05f,ratio * 3.0f);
	osg::Cone * upCone = new osg::Cone(VecUtils::Vec3Transform(properties.matrix, osg::Vec3d(ratio * 3.1f,0.0f,0.0f)),ratio * 0.15f,ratio * 0.5f);
	osg::Cylinder * leftCyl = new osg::Cylinder(VecUtils::Vec3Transform(properties.matrix, osg::Vec3d(0.0f,0.0f,ratio * 1.5f)),ratio * 0.05f,ratio * 3.0f);
	osg::Cone * leftCone = new osg::Cone(VecUtils::Vec3Transform(properties.matrix, osg::Vec3d(0.0f,0.0f,ratio * 3.1f)),ratio * 0.15f,ratio * 0.5f);
	
	osg::Matrixd m = osg::Matrixd::rotate( -osg::PI_2, osg::Vec3d( 1.0f, 0.0f, 0.0f ) ) * properties.matrix;
	headCyl->setRotation( m.getRotate() );
	headCone->setRotation( m.getRotate() );

	m = osg::Matrixd::rotate( osg::PI_2, osg::Vec3f( 0.0f, 1.0f, 0.0f ) ) * properties.matrix ;
	upCyl->setRotation( m.getRotate() );
	upCone->setRotation( m.getRotate() );

	m = osg::Matrixd::rotate( osg::PI_2, osg::Vec3f( 0.0f, 0.0f, 1.0f ) ) * properties.matrix ;
	leftCyl->setRotation( m.getRotate() );
	leftCone->setRotation( m.getRotate() );
	
	osg::ShapeDrawable * shape = new osg::ShapeDrawable(headCyl);
	shape->setColor( osg::Vec4f( 1.0f, 0.0f, 0.0f, 1.0f));
	geode->addDrawable( shape );
	geode->addDrawable( new osg::ShapeDrawable( headCone ));

	shape = new osg::ShapeDrawable(upCyl);
	shape->setColor( osg::Vec4f( 0.0f, 1.0f, 0.0f, 1.0f));
	geode->addDrawable( shape );
	geode->addDrawable( new osg::ShapeDrawable( upCone ));

	shape = new osg::ShapeDrawable(leftCyl);
	shape->setColor( osg::Vec4f( 0.0f, 0.0f, 1.0f, 1.0f));
	geode->addDrawable( shape );
	geode->addDrawable( new osg::ShapeDrawable( leftCone ));

	geode->addDrawable( new osg::ShapeDrawable(new osg::Sphere(VecUtils::Vec3Transform(properties.matrix,osg::Vec3d(0.0f,0.0f,0.0f)), ratio * 0.2f ) ) );
}

int MovingTurtle::makeRotate(osg::Quat & q)
{
	preRotate();

	osg::Matrixd x = osg::Matrixd::rotate( q ) ;

	properties.matrix =  x * properties.matrix;
	
	postRotate();

	//drawDebugGeometry( );

	return LS_OK;
}

int MovingTurtle::doStep( double dist)
{
	int ret = LS_NOTDEFINED;
	ret = preStep();

	if( !ret )
		properties.matrix = osg::Matrixd::translate( osg::Vec3d( 0.0, dist, 0.0 ) ) * properties.matrix;
	else
		return ret;

	drawDebugGeometry( );

	return postStep();
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
	this->doStep( properties.length / 2.0f );
	
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
		this->makeRotate( osg::Quat( - *(static_cast<double *>(p[0].value)), osg::Vec3d(1.0,0.0,0.0) ) ); 
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
		this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(1.0,0.0,0.0) ) ); 
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
		this->makeRotate( osg::Quat( properties.angle, osg::Vec3d(0.0,0.0,1.0) ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,0.0,1.0) ) ); 
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
		this->makeRotate( osg::Quat( - properties.angle, osg::Vec3d(0.0,0.0,1.0) ) ); 
		break;
	case 1:
		if (p[0].type != LS_DOUBLE)
			return LS_ERR_PAR_BADTYPE;
		this->makeRotate( osg::Quat( - *(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,0.0,1.0) ) ); 
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
		this->makeRotate( osg::Quat( - *(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) ); 
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
		this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) ); 
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

int MovingTurtle::increaseLength(std::vector<Parameter> & p)
{
	switch( p.size() )
	{
	case 0:
		this->properties.length *= properties.lengthIncrement;
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