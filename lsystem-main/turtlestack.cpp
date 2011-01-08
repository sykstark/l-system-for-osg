#include "precompiled.h"
#include "turtlestack.h"
#include "jointedpipe.h"
#include "straightpipe.h"
#include "rectangle.h"
#include "queryturtle.h"
#include "hermitpipe.h"
#include "log.h"

using namespace AP_LSystem;

TurtleStack::TurtleStack(void)
{
	
}

TurtleStack::~TurtleStack(void)
{
}

AbstractTurtle * TurtleStack::top()
{
	return m_Turtles.top();
}

int TurtleStack::push( )
{
	int res = LS_OK;
	LSGeode * geode = m_Turtles.top()->getGeode( );
	if ( !geode )
		return LS_ERR_STACK_NULL_LSGEODE;
	// create new turtle according to turtle type
	AbstractTurtle * pTurtleToPush = createTurtle(geode->getTurtleType());
	// turtle wasn't created
	if ( !pTurtleToPush )
		return LS_ERR_STACK_UNKNOWN_TURTLE_TYPE;
	// copy properties
	pTurtleToPush->setProperties( m_Turtles.top()->getProperties() );
	// bind with geode
	pTurtleToPush->bindGeode( geode );
	// initialize turtle
	res = pTurtleToPush->initialize( );
	if(res)
		return res;
	// push new one on the top of the stack
	m_Turtles.push( pTurtleToPush );
	return res;
}

int TurtleStack::push(LSGeode * geode)
{
	int res = LS_OK;
	// create new turtle that will replace the one on the top
	AbstractTurtle * pTurtle = createTurtle( geode->getTurtleType() );

	if( !pTurtle )
		return LS_ERR_STACK_UNKNOWN_TURTLE_TYPE;
	// bind with geode
	pTurtle->bindGeode( geode );
	// set default properties
	pTurtle->setProperties( geode->getDefaultTurtleProperties( ) );
	// inherit properties
	if( !m_Turtles.empty() )
		pTurtle->inheritProperties( m_Turtles.top()->getProperties() );
	// initialize turtle
	res = pTurtle->initialize( );
	if(res)
		return res;
	// push new one on the top of the stack
	m_Turtles.push( pTurtle );
	return res;
}

int TurtleStack::pop()
{
	int res = m_Turtles.top()->finalize();
	if(res)
		return res;
	// TODO delete top
	m_Turtles.pop();
	return res;
}

void TurtleStack::clear()
{
	while(!m_Turtles.empty())
		m_Turtles.pop();
}

AbstractTurtle * TurtleStack::createTurtle(AP_LSystem::TurtleType type)
{
	AbstractTurtle * turtle = NULL;

	Log::get()->incTurtleCounter();

	switch( type )
	{
	case LS_TURTLE_JOINTEDPIPE:
		turtle = new JointedPipe( );
		break;
	case LS_TURTLE_STRAIGHTPIPE:
		turtle = new StraightPipe( );
		break;
	case LS_TURTLE_HERMITPIPE:
		turtle = new HermitPipe( );
		break;
	case LS_TURTLE_RECTANGLE:
		turtle = new Rectangle( );
		break;
	case LS_TURTLE_QUERY:
		turtle = new QueryTurtle( );
		break;
	}

	return turtle;
}
