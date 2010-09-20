#pragma once

#ifndef ABSTRACTTURTLE_H_
#define ABSTRACTTURTLE_H_

namespace AP_LSystem {
class AbstractTurtle
{
private:
	TurtleProperties
public:
//****************************************************************
//**						OTHER								**
//****************************************************************
	virtual bool resetValues()=0;
	virtual TurtleProperties getProperties();
	virtual bool setProperties( TurtleProperties &);
//****************************************************************
//**						ROTATION							**
//****************************************************************
	virtual bool turnLeft()									= 0;
	virtual bool turnLeft(double)							= 0;
	virtual bool turnRight()								= 0;
	virtual bool turnRight(double)							= 0;
	virtual bool pitchDown()								= 0;
	virtual bool pitchDown(double)							= 0;
	virtual bool pitchUp()									= 0;
	virtual bool pitchUp(double)							= 0;
	virtual bool rollLeft()									= 0;
	virtual bool rollLeft(double)							= 0;
	virtual bool rollRight()								= 0;
	virtual bool rollRight(double)							= 0;

	virtual bool turnArround()								= 0;
	virtual bool rollArround()								= 0;
	virtual bool rollUntilHorizontal()						= 0;
	virtual bool randomTurnPitchRoll()						= 0;
	virtual bool randomTurnPitchRoll(unsigned int angle)	= 0;
	virtual bool gravityCorrection(double ratio)			= 0;

//****************************************************************
//**						MOVEMENT							**
//****************************************************************

	virtual bool drawForward()								= 0;
	virtual bool drawForward(double length)					= 0;
	virtual bool moveForward()								= 0;
	virtual bool moveForward(double length)					= 0;
	
};

struct TurtleProperties
{
	double length;
	double angle;
	double thickness;
	unsigned int detailH;
	unsigned int detailL;
	unsigned int detailU;
};
}

#endif