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

}

int DefaultParser::parse(ParseableString * word)
{
	int currentGrammarIndex = 0;
	vector<Parameter> parameters;

	while(!word->eof( ))
	{
		// FREE SYMBOLS
		// ! " # % ' * , . : ; < > @ [ ] _ ` { } ? ~
		parameters.clear();
		switch( word->next( parameters ) )
		{
		case 'F':
			turtles.top()->drawForward( parameters );
			break;
		case 'Z':
			turtles.top()->moveForwardHalf( );
			break;
		case '+':
			turtles.top()->turnLeft( parameters );
			break;
		case '-':
			turtles.top()->turnRight( parameters );
			break;
		case '&':
			turtles.top()->pitchDown( parameters );
			break;
		case '^':
			turtles.top()->pitchUp( parameters );
			break;
		case '\\':
			turtles.top()->rollLeft( parameters );
			break;
		case '/':
			turtles.top()->rollRight( parameters );
			break;
		case '|':
			turtles.top()->turnArround( );
			break;
		case '=':
			turtles.top()->rollUntilHorizontal( );
			break;
		case '\'':
			turtles.top()->increaseLength( parameters );
			break;
		case '!':
			turtles.top()->increaseRadius( parameters );
			break;
		case '$':
			switchGeode( parameters );
			break;
		case '[':
			turtles.push( selectedGeode );
			break;
		case ']':
			turtles.pop( );
			break;
		}	
	}
	return 0;
}
