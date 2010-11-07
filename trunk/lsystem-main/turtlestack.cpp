#include "precompiled.h"
#include "turtlestack.h"
#include "jointedpipe.h"
#include "straightpipe.h"
#include "rectangle.h"
#include "queryturtle.h"

using namespace AP_LSystem;

TurtleStack::TurtleStack(void)
{
	
}

TurtleStack::~TurtleStack(void)
{
}

AbstractTurtle * TurtleStack::top()
{
	return turtles.top();
}

void TurtleStack::push( )
{
	LSGeode * geode = turtles.top()->getGeode( );
	// create new turtle according to turtle type
	AbstractTurtle * pTurtleToPush = createTurtle(geode->getTurtleType());
	// copy properties
	pTurtleToPush->setProperties( turtles.top()->getProperties() );
	// bind with geode
	pTurtleToPush->bindGeode( geode );
	// initialize turtle
	pTurtleToPush->initialize( );
	// push new one on the top of the stack
	turtles.push( pTurtleToPush );
}

void TurtleStack::pop()
{
	turtles.top()->finalize();
	// TODO delete top
	turtles.pop();
}

void TurtleStack::push(LSGeode * geode)
{
	// create new turtle that will replace the one on the top
	AbstractTurtle * pTurtle = createTurtle( geode->getTurtleType() );
	// bind with geode
	pTurtle->bindGeode( geode );
	// set default properties
	pTurtle->setProperties( geode->getDefaultTurtleProperties( ) );
	// inherit properties
	if( !turtles.empty() )
		pTurtle->inheritProperties( turtles.top()->getProperties() );
	// initialize turtle
	pTurtle->initialize( );
	// push new one on the top of the stack
	turtles.push( pTurtle );
}

AbstractTurtle * TurtleStack::createTurtle(AP_LSystem::TurtleType type)
{
	AbstractTurtle * turtle;

	switch( type )
	{
	case LS_TURTLE_JOINTEDPIPE:
		turtle = new JointedPipe( );
		break;
	case LS_TURTLE_STRAIGHTPIPE:
		turtle = new StraightPipe( );
		break;
	case LS_TURTLE_RECTANGLE:
		turtle = new Rectangle( );
		break;
	case LS_TURTLE_QUERY:
		turtle = new QueryTurtle( );
		break;
	// TODO other types
	}

	return turtle;
}
