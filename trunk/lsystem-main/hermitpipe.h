#ifndef HERMITPIPE_H_
#define HERMITPIPE_H_

#include "straightpipe.h"

namespace AP_LSystem {
class HermitPipe : public StraightPipe
{
private:
	void initializePipe();
	void finalizePipe();
	void drawCap();
public:
	HermitPipe(void){};
	~HermitPipe(void){};

	virtual int drawStep( double dist );

	virtual int initialize( );
	virtual int finalize( );

};
}

#endif