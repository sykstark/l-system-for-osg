#include "precompiled.h"

#include "queryinterpret.h"

using namespace AP_LSystem;

QueryInterpret * QueryInterpret::interpret = 0;

int QueryInterpret::parse( ParseableString * word)
{
	// There won't be any subsystems, because quering is processed during derivation process for each
	// l-system separately. And inserting subsystems into parent's word is part of final translation
	// of word - ie. after whole derivation process.
	vector<Parameter> parameters;

	// TODO maybe specify
	m_Turtles.push( m_Geodes[0] );

	while(!word->eof( ))
	{
		// FREE SYMBOLS
		// ! " # % ' * , . : ; < > @ _ ` ? ~
		parameters.clear();
		switch( word->next( parameters ) )
		{
		case 'F':
		case 'f':
			m_Turtles.top()->moveForward( parameters );
			break;
		case 'Z':
		case 'z':
			m_Turtles.top()->moveForwardHalf( );
			break;
		case '+':
			m_Turtles.top()->turnLeft( parameters );
			break;
		case '-':
			m_Turtles.top()->turnRight( parameters );
			break;
		case '&':
			m_Turtles.top()->pitchDown( parameters );
			break;
		case '^':
			m_Turtles.top()->pitchUp( parameters );
			break;
		case '\\':
			m_Turtles.top()->rollLeft( parameters );
			break;
		case '/':
			m_Turtles.top()->rollRight( parameters );
			break;
		case '|':
			m_Turtles.top()->turnArround( );
			break;
		case '=':
			m_Turtles.top()->rollUntilHorizontal( );
			break;
		case '[':
			m_Turtles.push( );
			break;
		case ']':
			m_Turtles.pop( );
			break;
		}	
	}
	
	// store position matrix of turtle
	matrix = m_Turtles.top()->getMatrix();
	m_Turtles.clear();
	
	return 0;
}