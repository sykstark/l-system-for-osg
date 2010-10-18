#include "precompiled.h"
#include "defaultparser.h"

#include "Configuration.h"

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
		// ! " # % ' * , . : ; < > @ [ ] _ ` { } ~
		parameters.clear();
		switch( word->next( parameters ) )
		{
		case 'F':
			turtles.top()->moveForward( parameters );
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
		case '?':
			if (parameters[0].type != LS_UBYTE)
				return LS_ERR_PAR_BADTYPE;
			switchGeode( *(static_cast<unsigned char*>(parameters[0].value)));
			break;
		case '$':
			break;
		case '(':
			turtles.push( selectedGeode );
			break;
		case ')':
			turtles.pop( );
			break;
		}	
	}
	return 0;
}
