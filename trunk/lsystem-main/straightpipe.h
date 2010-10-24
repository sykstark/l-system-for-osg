#ifndef STRAIGHTPIPE_H_
#define STRAIGHTPIPE_H_

#include "loftturtle.h"

namespace AP_LSystem {
class StraightPipe : public LoftTurtle
{
private:
	void initializePipe();
public:
	StraightPipe(void){};
	~StraightPipe(void){};

	virtual int insideStep( );

	virtual void setProperties( TurtleProperties p );

	virtual int drawForward( std::vector<Parameter> & ) {return 0;}

};
}

#endif