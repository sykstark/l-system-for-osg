#ifndef HERMITPIPE_H_
#define HERMITPIPE_H_

#include "loftturtle.h"

namespace AP_LSystem {
class HermitPipe : public LoftTurtle
{
private:
	void initializePipe();
	void finalizePipe();
	void drawCap();
public:
	HermitPipe(void){};
	~HermitPipe(void){};

	virtual int insideStep( );

	virtual int initialize( );
	virtual int finalize( );

};
}

#endif