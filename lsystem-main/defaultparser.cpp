#include "precompiled.h"
#include "defaultparser.h"

#include "Configuration.h"

using namespace AP_LSystem;

int DefaultParser::parse(ParseableString * word)
{
	int currentGrammarIndex = 0;
	vector<Parameter> parameters;

	while(!word->eof( ))
	{
		// FREE SYMBOLS
		// ! " # $ % ' * , . : ; < > ? @ [ ] _ ` { } ~
		parameters.clear();
		switch( word->next( parameters ) )
		{
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
			if (parameters[0].type != LS_BYTE)
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
