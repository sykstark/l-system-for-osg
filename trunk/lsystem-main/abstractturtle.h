#ifndef ABSTRACTTURTLE_H_
#define ABSTRACTTURTLE_H_

#include "StringUtils.h"
#include "lsgeode.h"

namespace AP_LSystem {
//class LSGeode;
//struct TurtleProperties;

enum ParsingErrorCodes
{
	LS_OK,
	LS_NOTDEFINED,
	LS_ERR_PAR_BADTYPE,
	LS_ERR_PAR_INVALIDCOUNT,
};
/**
 *	Flags of turtle settings
 */
enum TurtleFlag
{
	LS_FL_DEBUG				= 0x00000001,

};

/**
 *	Abstract class for all turtles with declarations of all necessary functions.
 */
class AbstractTurtle
{
protected:
	TurtleProperties properties;	///< properties of turtle is used for getting parameters for generation of geometry
	LSGeode * geode;				///< all geometry generated by turtle is stored to this output geode	
public:
//****************************************************************
//**						OTHER								**
//****************************************************************
	virtual int resetValues() { return 0;}//TODO	///< reset all values in properties to default value as set in Configuration
	TurtleProperties & getProperties()	///< get Properties reference
	{
		return properties;
	}

	void setProperties( TurtleProperties p )	///< set Properties of turtle. Must be part of turtle initialization
	{
		properties = p;
	}

	void bindGeode( LSGeode * geode )	///< bind output geode with current turtle 
	{
		this->geode = geode;
	}

//****************************************************************
//**						ROTATION							**
//****************************************************************
	virtual int turnLeft(std::vector<Parameter> &)			= 0;
	virtual int turnRight(std::vector<Parameter> &)			= 0;
	virtual int pitchDown(std::vector<Parameter> &)			= 0;
	virtual int pitchUp(std::vector<Parameter> &)			= 0;
	virtual int rollLeft(std::vector<Parameter> &)			= 0;
	virtual int rollRight(std::vector<Parameter> &)			= 0;

	virtual int turnArround()								= 0;
	virtual int rollArround()								= 0;
	virtual int rollUntilHorizontal()						= 0;
	virtual int randomTurnPitchRoll(std::vector<Parameter>&)= 0;
	virtual int tropism(std::vector<Parameter> &)	= 0;

//****************************************************************
//**						MOVEMENT							**
//****************************************************************

	virtual int drawForward(std::vector<Parameter> &)		= 0;
	virtual int moveForward(std::vector<Parameter> &)		= 0;
	
};
}

#endif