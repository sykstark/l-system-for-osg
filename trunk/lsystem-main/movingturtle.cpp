#include "precompiled.h"

#include "movingturtle.h"

using namespace AP_LSystem;

void MovingTurtle::drawDebugGeometry( )
{
/*	if( ! (properties.flags & LS_FL_DEBUG ) )
		return;*/

	float ratio = 0.5;

	osg::Cylinder * headCyl = new osg::Cylinder(VecUtils::Vec3Transform(properties.matrix, osg::Vec3d(0.0f,ratio * 1.5f,0.0f)),ratio * 0.05f,ratio * 3.0f);
	osg::Cone * headCone = new osg::Cone(osg::Vec3(0.0f,ratio * 3.1f,0.0f),ratio * 0.15f,ratio * 0.5f);
	osg::Cylinder * upCyl = new osg::Cylinder(osg::Vec3(ratio * 1.5f,0.0f,0.0f),ratio * 0.05f,ratio * 3.0f);
	osg::Cone * upCone = new osg::Cone(osg::Vec3(ratio * 3.1f,0.0f,0.0f),ratio * 0.15f,ratio * 0.5f);
	osg::Cylinder * leftCyl = new osg::Cylinder(osg::Vec3(0.0f,0.0f,ratio * 1.5f),ratio * 0.05f,ratio * 3.0f);
	osg::Cone * leftCone = new osg::Cone(osg::Vec3(0.0f,0.0f,ratio * 3.1f),ratio * 0.15f,ratio * 0.5f);
	headCyl->setRotation( osg::Quat( osg::PI_2, osg::Vec3f( 1.0f, 0.0f, 0.0f ) ) );
	headCone->setRotation( osg::Quat( -osg::PI_2, osg::Vec3f( 1.0f, 0.0f, 0.0f ) ) );
	upCyl->setRotation( osg::Quat( osg::PI_2, osg::Vec3f( 0.0f, 1.0f, 0.0f ) ) );
	upCone->setRotation( osg::Quat( osg::PI_2, osg::Vec3f( 0.0f, 1.0f, 0.0f ) ) );
	leftCyl->setRotation( osg::Quat( osg::PI_2, osg::Vec3f( 0.0f, 0.0f, 1.0f ) ) );
	leftCone->setRotation( osg::Quat( -osg::PI_2, osg::Vec3f( 0.0f, 0.0f, 1.0f ) ) );
	
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

	geode->addDrawable( new osg::ShapeDrawable(new osg::Sphere(osg::Vec3f(0.0f,0.0f,0.0f), ratio * 0.2f ) ) );
}

int MovingTurtle::makeRotate(osg::Quat & q)
{
	preRotate();

	properties.matrix.makeRotate( q );
	
	postRotate();

	return LS_OK;
}

int MovingTurtle::doStep( double dist)
{
	int ret = LS_NOTDEFINED;
	ret = preStep();

	if( !ret )
		properties.matrix.makeTranslate( osg::Vec3d( dist, 0.0, 0.0 ) );
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

int MovingTurtle::turnLeft(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_DOUBLE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( - *(static_cast<double *>(p[0].value)), osg::Vec3d(1.0,0.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::turnRight(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_DOUBLE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(1.0,0.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::pitchDown(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_DOUBLE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( -*(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::pitchUp(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_DOUBLE)
		return LS_ERR_PAR_BADTYPE;
	
	this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::rollLeft(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_DOUBLE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( -*(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) );


	return LS_OK;
}

int MovingTurtle::rollRight(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_DOUBLE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) );


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