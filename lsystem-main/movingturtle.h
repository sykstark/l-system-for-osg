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
	virtual int turnLeft(std::vector<Parameter> &);
	virtual int turnRight(std::vector<Parameter> &);
	virtual int pitchDown(std::vector<Parameter> &);
	virtual int pitchUp(std::vector<Parameter> &);
	virtual int rollLeft(std::vector<Parameter> &);
	virtual int rollRight(std::vector<Parameter> &);

	virtual int turnArround();
	virtual int rollArround();
	virtual int rollUntilHorizontal();
	virtual int randomTurnPitchRoll(std::vector<Parameter> &);
	virtual int tropism(std::vector<Parameter> &);

//****************************************************************
//**				    CHANGE PROPERTIES						**
//****************************************************************
	virtual int multiplyLength(std::vector<Parameter> &);
	virtual int multiplyRadius(std::vector<Parameter> &);
	virtual int multiplyAngle(std::vector<Parameter> &);

//****************************************************************
//**						MOVEMENT							**
//****************************************************************
	virtual int drawForward(std::vector<Parameter> &);
	virtual int drawForwardHalf( );
	virtual int moveForward(std::vector<Parameter> &);
	virtual int moveForwardHalf( );
	
	
};
}

#endif