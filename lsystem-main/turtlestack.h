#ifndef TURTLESTACK_H_
#define TURTLESTACK_H_

namespace AP_LSystem {
class TurtleStack
{
private:
	stack< AbstractTurtle * > turtles;
public:
	TurtleStack(void);
	~TurtleStack(void);

	void push( LSGeode * geode );
	void pop( );
	AbstractTurtle * top( );
};
}

#endif