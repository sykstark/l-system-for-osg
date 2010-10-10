#ifndef MOVINGTURTLE_H_
#define MOVINGTURTLE_H_

#include "abstractturtle.h"

namespace AP_LSystem {
class MovingTurtle : public AbstractTurtle
{
private:
	virtual int makeRotate( osg::Quat & );
	virtual int preRotate()=0;
	virtual int postRotate()=0;

	virtual int doStep( osg::Quat & );
	virtual int preStep()=0;
	virtual int postStep()=0;

	void drawDebugGeometry( );
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
//**						MOVEMENT							**
//****************************************************************
	virtual int moveForward(std::vector<Parameter> &);
	
};
}

#endif