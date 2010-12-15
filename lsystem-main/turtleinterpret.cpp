#include "precompiled.h"
#include "turtleinterpret.h"

#include "configuration.h"

using namespace AP_LSystem;

TurtleInterpret::TurtleInterpret( osg::Group * owner )
{
	// TODO move to abstract
	if( owner )
	{
		pOwner = owner;
		createGeodes();
	}
}

TurtleInterpret::~TurtleInterpret( )
{
	// TODO delete geodes

}

int TurtleInterpret::parse(ParseableString * word)
{
	int res = 0;
	vector<Parameter> parameters;

	while(!word->eof( ))
	{

		// FREE SYMBOLS
		// ! " # % ' * , . : ; < > @ [ ] _ ` { } ? ~
		parameters.clear();
		switch( word->next( parameters ) )
		{
		case 'F':
			res = turtles.top()->drawForward( parameters );
			break;
		case 'Z':
			res = turtles.top()->drawForwardHalf( );
			break;
		case 'f':
			res = turtles.top()->moveForward( parameters );
			break;
		case 'z':
			res = turtles.top()->moveForwardHalf( );
			break;
		case '+':
			res = turtles.top()->turnLeft( parameters );
			break;
		case '-':
			res = turtles.top()->turnRight( parameters );
			break;
		case '&':
			res = turtles.top()->pitchDown( parameters );
			break;
		case '^':
			res = turtles.top()->pitchUp( parameters );
			break;
		case '\\':
			res = turtles.top()->rollLeft( parameters );
			break;
		case '/':
			res = turtles.top()->rollRight( parameters );
			break;
		case '|':
			res = turtles.top()->turnArround( );
			break;
		case '=':
			res = turtles.top()->rollUntilHorizontal( );
			break;
		case '\'':
			res = turtles.top()->multiplyLength( parameters );
			break;
		case '!':
			res = turtles.top()->multiplyRadius( parameters );
			break;
		case '$':
			res = switchGeode( parameters );
			break;
		case '[':
			turtles.push( );
			break;
		case ']':
			turtles.pop( );
			break;
		}	

		if ( res )
			return res;
	}
	return LS_OK;
}
