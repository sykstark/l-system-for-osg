#ifndef MOVINGTURTLE_H_
#define MOVINGTURTLE_H_

#include "abstractturtle.h"

namespace AP_LSystem {
class MovingTurtle : public AbstractTurtle
{
protected:
	virtual int makeRotate( osg::Quat & );
	virtual int preRotate() { return 0; }
	virtual int postRotate() { return 0; }

	virtual int doStep( double );
	virtual int drawStep( double );
	virtual int preStep(){ return 0; }
	virtual int postStep(){ return 0; }

	virtual void drawFrame( osg::Matrixd &, osg::Vec4d * = NULL);
	virtual void adjustMatrices( );
public:
//****************************************************************
//**						ROTATION							**
//****************************************************************
	/**
	  *
	  */
	virtual int turnLeft(std::vector<Parameter> & p);
	virtual int turnRight(std::vector<Parameter> & p);
	virtual int pitchDown(std::vector<Parameter> & p);
	virtual int pitchUp(std::vector<Parameter> & p);
	virtual int rollLeft(std::vector<Parameter> & p);
	virtual int rollRight(std::vector<Parameter> & p);

	virtual int turnArround();
	virtual int rollArround();
	virtual int rollUntilHorizontal();
	virtual int randomTurnPitchRoll(std::vector<Parameter> & p);

//****************************************************************
//**				    CHANGE PROPERTIES						**
//****************************************************************
	virtual int multiplyLength(std::vector<Parameter> & p);
	virtual int multiplyRadius(std::vector<Parameter> & p);
	virtual int multiplyAngle(std::vector<Parameter> & p);
	virtual int multiplyTropismElasticity(std::vector<Parameter> & p);
	virtual int multiplyGravitropismElasticity(std::vector<Parameter> & p);

//****************************************************************
//**						MOVEMENT							**
//****************************************************************
	virtual int drawForward(std::vector<Parameter> & p);
	virtual int drawForwardHalf( );
	virtual int moveForward(std::vector<Parameter> & p);
	virtual int moveForwardHalf( );
	
	
};
}

#endif