#include "precompiled.h"

#include "movingturtle.h"

using namespace AP_LSystem;

void MovingTurtle::drawDebugGeometry( )
{
	if( ! (properties.flags |= LS_FL_DEBUG ) )
		return;


}

int MovingTurtle::makeRotate(osg::Quat & q)
{
	preRotate();

	properties.matrix.makeRotate( q );
	
	postRotate();

	return LS_OK;
}

int MovingTurtle::doStep()
{
	int ret = LS_NOTDEFINED;
	ret = preStep();

	if( !ret )
		ret = properties.matrix.makeTranslate( osg::Vec3d( dist, 0.0, 0.0 ) );
	else
		return ret;

	return postStep();
}

int MovingTurtle::moveForward(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_BYTE)
		return LS_ERR_PAR_BADTYPE;

	return LS_OK;
}

int MovingTurtle::turnLeft(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_BYTE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( - *(static_cast<double *>(p[0].value)), osg::Vec3d(1.0,0.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::turnRight(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_BYTE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(1.0,0.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::pitchDown(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_BYTE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( -*(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::pitchUp(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_BYTE)
		return LS_ERR_PAR_BADTYPE;
	
	this->makeRotate( osg::Quat( *(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) ); 

	return LS_OK;
}

int MovingTurtle::rollLeft(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_BYTE)
		return LS_ERR_PAR_BADTYPE;

	this->makeRotate( osg::Quat( -*(static_cast<double *>(p[0].value)), osg::Vec3d(0.0,1.0,0.0) ) );


	return LS_OK;
}

int MovingTurtle::rollRight(std::vector<Parameter> & p)
{
	if (p.size() != 1)
		return LS_ERR_PAR_INVALIDCOUNT;
	if (p[0].type != LS_BYTE)
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