#include "precompiled.h"
#include "turtleinterpret.h"

#include "configuration.h"

using namespace AP_LSystem;

TurtleInterpret::TurtleInterpret( osg::Group * owner )
{
	if( owner )
	{
		m_Owner = owner;
		createGeodes();
	}
}

TurtleInterpret::~TurtleInterpret( )
{

}

int TurtleInterpret::parse(ParseableString * word)
{
	int res = 0;
	vector<Parameter> parameters;

	while(!word->eof( ))
	{

		// FREE SYMBOLS
		// " * , . < > @ _ ` { } ? ~
		parameters.clear();
		switch( word->next( parameters ) )
		{
		case 'F':
			res = m_Turtles.top()->drawForward( parameters );
			break;
		case 'Z':
			res = m_Turtles.top()->drawForwardHalf( );
			break;
		case 'f':
			res = m_Turtles.top()->moveForward( parameters );
			break;
		case 'z':
			res = m_Turtles.top()->moveForwardHalf( );
			break;
		case '+':
			res = m_Turtles.top()->turnLeft( parameters );
			break;
		case '-':
			res = m_Turtles.top()->turnRight( parameters );
			break;
		case '&':
			res = m_Turtles.top()->pitchDown( parameters );
			break;
		case '^':
			res = m_Turtles.top()->pitchUp( parameters );
			break;
		case '\\':
			res = m_Turtles.top()->rollLeft( parameters );
			break;
		case '/':
			res = m_Turtles.top()->rollRight( parameters );
			break;
		case '|':
			res = m_Turtles.top()->turnArround( );
			break;
		case '=':
			res = m_Turtles.top()->rollUntilHorizontal( );
			break;
		case ':':
			res = m_Turtles.top()->multiplyAngle( parameters );
			break;
		case '\'':
			res = m_Turtles.top()->multiplyLength( parameters );
			break;
		case '!':
			res = m_Turtles.top()->multiplyRadius( parameters );
			break;
		case ';':
			res = m_Turtles.top()->multiplyTropismElasticity( parameters );
			break;
		case '~':
			res = m_Turtles.top()->multiplyGravitropismElasticity( parameters );
			break;
		case '$':
			res = switchGeode( parameters );
			break;
		case '[':
			m_Turtles.push( );
			break;
		case ']':
			m_Turtles.pop( );
			break;
		}	

		if ( res )
			return res;
	}
	return LS_OK;
}
