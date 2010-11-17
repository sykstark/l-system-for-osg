#include "precompiled.h"
#include "defaultparser.h"

#include "configuration.h"

using namespace AP_LSystem;

DefaultParser::DefaultParser( osg::Group * owner )
{
	pOwner = owner;
	createGeodes();
}

DefaultParser::~DefaultParser( )
{
	// TODO delete geodes

}

int DefaultParser::parse(ParseableString * word)
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
