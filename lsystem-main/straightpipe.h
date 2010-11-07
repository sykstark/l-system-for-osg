#ifndef STRAIGHTPIPE_H_
#define STRAIGHTPIPE_H_

#include "loftturtle.h"

namespace AP_LSystem {
class StraightPipe : public LoftTurtle
{
private:
	void initializePipe();
	void finalizePipe();
public:
	StraightPipe(void){};
	~StraightPipe(void){};

	virtual int preStep( );
	virtual int insideStep( );

	//virtual void setProperties( TurtleProperties p );

	virtual int initialize( );
	virtual int finalize( );

};
}

#endif