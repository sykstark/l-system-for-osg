#ifndef QUERYINTERPRET_H_
#define QUERYINTERPRET_H_

#include "abstractparser.h"
#include "lsgeode.h"

namespace AP_LSystem {
class QueryInterpret : public AbstractParser
{
private:
	static QueryInterpret * interpret;

	QueryInterpret( osg::Group * owner ){};
	//QueryInterpret( );
	~QueryInterpret(void);
	
public:
	static QueryInterpret * get()
	{
		if(!interpret) interpret = new QueryInterpret();

		return interpret;
	}

	virtual int parse( ParseableString * )
	{
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
			case '[':
				turtles.push( );
				break;
			case ']':
				turtles.pop( );
				break;
			}	
		}
		return 0;
	}
};

QueryInterpret * QueryInterpret::interpret = 0;
}

#endif