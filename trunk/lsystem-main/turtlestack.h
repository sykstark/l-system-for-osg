#ifndef TURTLESTACK_H_
#define TURTLESTACK_H_

#include <stack>
#include "abstractturtle.h"
#include "lsgeode.h"

using std::stack;

namespace AP_LSystem {

class TurtleStack
{
private:
	stack< AbstractTurtle * > turtles;

	AbstractTurtle * createTurtle( TurtleType type );
public:
	TurtleStack(void);
	~TurtleStack(void);

	int push( );					///< push turtle of defined type on the stack
	int pop( );					///< pop one turtle	
	
	/**
	 *	Replaces turtle on the top of the stack by another turtle. Type of turtle is determined from LSGeode.
	 *	Turtle is binded with LSGeode to write there a geometry.
	 *	@param TODO
	 *	@return error code
	 */
	int push( LSGeode * );
	
	/**
	 *	Get turtle from the top of the stack
	 *	@return turtle from the top of the stack
	 */
	AbstractTurtle * top( );


};
}

#endif