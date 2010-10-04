#include "precompiled.h"
#include "defaultparser.h"

#include "Configuration.h"

using namespace AP_LSystem;

void DefaultParser::parse(ParseableString * word)
{
	while(!word->eof( ))
	{
		vector<Parameter> parameters;
		switch( word->next( parameters ) )
		{
		case 'F':
			break;
		case 'A':
			break;
		case '(':
			pushTurtle( );
			break;
		case ')':
			popTurtle( );
			break;
		}
	}
}
