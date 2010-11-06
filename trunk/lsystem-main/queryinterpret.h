#ifndef QUERYINTERPRET_H_
#define QUERYINTERPRET_H_

#include "abstractparser.h"
#include "lsgeode.h"

namespace AP_LSystem {
class QueryInterpret : public AbstractParser
{
private:
	static QueryInterpret * interpret;
	osg::Matrixd matrix;
	unsigned dirty;

	LongString * word;

	enum dirtyBits
	{
		CLEAR			= 0x00000000,
		POSITION		= 0x0000000F,
		POSITIONX		= 0x00000001,
		POSITIONY		= 0x00000002,
		POSITIONZ		= 0x00000004,
		HEADING			= 0x000000F0,
		HEADINGX		= 0x00000010,
		HEADINGY		= 0x00000020,
		HEADINGZ		= 0x00000040,
		UP				= 0x00000F00,
		UPX				= 0x00000100,
		UPY				= 0x00000200,
		UPZ				= 0x00000400,
		LEFT			= 0x0000F000,
		LEFTX			= 0x00001000,
		LEFTY			= 0x00002000,
		LEFTZ			= 0x00004000,
		DIRTY			= 0xFFFFFFFF,
	};

	QueryInterpret()
	{
		// create group for geodes
		pOwner = new osg::Group();

		word = NULL;

		createGeodes();

		dirty = QueryInterpret::DIRTY;

		for( vector<LSGeode *>::iterator it = geodes.begin(); it != geodes.end(); it++ )
		{
			(*it)->setTurtleType( LS_TURTLE_QUERY );
		}
	};

	void update()
	{
		ParseableString * pW = new ParseableString( word );
			
		if(!pW)
			return;

		parse( pW );
	}

	virtual int parse( ParseableString * word)
	{
		// There won't be any subsystems, because quering is processed during derivation process for each
		// l-system separately. And inserting subsystems into parent's word is part of final translation
		// of word - ie. after whole derivation process.
		vector<Parameter> parameters;

		// TODO maybe specify
		turtles.push( geodes[0] );

		while(!word->eof( ))
		{
			// FREE SYMBOLS
			// ! " # % ' * , . : ; < > @ _ ` ? ~
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
		// store complete matrix of turtle
		matrix = turtles.top()->getMatrix();
		return 0;
	}
	//QueryInterpret( );
	~QueryInterpret(void);
	
public:
	static QueryInterpret * get()
	{
		if(!interpret) interpret = new QueryInterpret();

		return interpret;
	}


	double positionX( )
	{
		if( dirty & QueryInterpret::POSITIONX )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::POSITION;
		}
		dirty |= QueryInterpret::POSITIONX;

		return matrix.getTrans().x();
	}

	double positionY( )
	{
		if( dirty & QueryInterpret::POSITIONY )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::POSITION;
		}
		dirty |= QueryInterpret::POSITIONY;

		return matrix.getTrans().y();
	}

	double positionZ( )
	{
		if( dirty & QueryInterpret::POSITIONZ )
		{
			// update matrix
			update();

			// set Position as CLEAR
			dirty &= ~QueryInterpret::POSITION;
		}
		dirty |= QueryInterpret::POSITIONZ;

		return matrix.getTrans().z();
	}

	void bindWord( LongString * str)
	{
		word = str;
	}
	
};
}

#endif

