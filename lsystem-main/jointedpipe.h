#ifndef JOINTEDPIPE_H_
#define JOINTEDPIPE_H_

#include "loftturtle.h"

namespace AP_LSystem {
class JointedPipe : public LoftTurtle
{
public:
	JointedPipe(void);
	~JointedPipe(void);

	virtual int drawForward( std::vector<Parameter> & ) {return 0;}

	virtual int preRotate() { return 0;}
	virtual int postRotate() { return 0;}
};
}

#endif