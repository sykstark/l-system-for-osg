#include "precompiled.h"
#include "turtlestack.h"
#include "jointedpipe.h"

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

void TurtleStack::push( LSGeode * geode )
{
	// create new turtle accroding to turtle type
	AbstractTurtle * pTurtleToPush = createTurtle(geode->getTurtleType());
	// copy properties
	pTurtleToPush->setProperties( turtles.top()->getProperties() );
	// bind with geode
	pTurtleToPush->bindGeode( geode );
	// push new one on the top of the stack
	turtles.push( pTurtleToPush );
}

void TurtleStack::pop()
{
	// TODO delete top
	turtles.pop();
}

void TurtleStack::replace(LSGeode * geode)
{
	// create new turtle that will replace the one on the top
	AbstractTurtle * pTurtle = createTurtle( geode->getTurtleType() );
	// copy properties
	pTurtle->setProperties( geode->getDefaultTurtleProperties( ) );
	// bind with geode
	pTurtle->bindGeode( geode );
	// push new one on the top of the stack
	turtles.push( pTurtle );
}

AbstractTurtle * TurtleStack::createTurtle(AP_LSystem::TurtleType type)
{
	AbstractTurtle * turtle;

	switch( type )
	{
	case LS_TURTLE_JOINTEDPIPE:
		turtle = new JointedPipe(  );
	// TODO other types
	}

	return turtle;
}
