#pragma once

class AbstractTurtle
{
public:

	AbstractTurtle(void)
	{
	}

	~AbstractTurtle(void)
	{
	}

//****************************************************************
//**						ROTATION							**
//****************************************************************
	bool turnLeft()									= 0;
	bool turnLeft(double angle)						= 0;
	bool turnRight()								= 0;
	bool turnRight(double angle)					= 0;
	bool pitchDown()								= 0;
	bool pitchDown(double angle)					= 0;
	bool pitchUp()									= 0;
	bool pitchUp(double angle)						= 0;
	bool rollLeft()									= 0;
	bool rollLeft(double angle)						= 0;
	bool rollRight()								= 0;
	bool rollRight(double angle)					= 0;

	bool turnArround()								= 0;
	bool rollArround()								= 0;
	bool rollUntilHorizontal()						= 0;
	bool randomTurnPitchRoll()						= 0;
	bool randomTurnPitchRoll(unsigned int angle)	= 0;
	bool gravityCorrection(double ratio)			= 0;

//****************************************************************
//**						MOVEMENT							**
//****************************************************************

	virtual bool resetValues()=0;

	bool fullForwardDrawMove();
	bool halfForwardDrawMove();
	virtual bool forwardDrawMove(double length)=0;

	bool fullForwardMove();
	bool forwardMove(double length);
	
};
