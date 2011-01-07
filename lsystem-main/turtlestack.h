#ifndef TURTLESTACK_H_
#define TURTLESTACK_H_

#include <stack>
#include "abstractturtle.h"
#include "lsgeode.h"

using std::stack;

namespace AP_LSystem {

/**
 *	Specialized stack for turtle processing.
 */
class TurtleStack
{
private:
    stack< AbstractTurtle * > turtles;      ///< stack with turtles

    /**
     *	Creates an instance of turtle of selected type.
     *	@param type Type of turtle to be created.
     *	@return error code
     */
    AbstractTurtle * createTurtle( TurtleType type );
public:
	TurtleStack(void);
	~TurtleStack(void);

    /**
     *	Pops a turtle on the top of the stack.
     *	@return error code
     */
    int pop( );

    /**
     *	Adds turtle on the top of the stack. The turtle will have same type as current turtle on the top
     *  of the stack.
     *	Turtle is also binded with the same LSGeode.
     *	@param geode Inserted turtle will be binded with this geode.
     *	@return error code
     */
    int push( );
	
    /**
     *	Adds turtle of different type on the top of the stack. Type of the turtle is determined from LSGeode.
     *	Turtle is binded with LSGeode to write there a geometry.
     *	@param geode Inserted turtle will be binded with this geode.
     *	@return error code
     */
    int push( LSGeode * geode);
	
	/**
     *	Get turtle from the top of the stack.
	 *	@return turtle from the top of the stack
	 */
	AbstractTurtle * top( );

    /**
     *	Removes all turtles
     */
	void clear( );
};
}

#endif
