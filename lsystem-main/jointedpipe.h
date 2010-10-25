#ifndef JOINTEDPIPE_H_
#define JOINTEDPIPE_H_

#include "loftturtle.h"

namespace AP_LSystem {
class JointedPipe : public LoftTurtle
{
private:
	void createHemisphere();
public:
	JointedPipe(void);
	~JointedPipe(void);

	virtual int drawStep(double dist);

	virtual void setProperties( TurtleProperties p );

	virtual int preRotate() { return 0;}
	virtual int postRotate() { return 0;}
	virtual int preStep();
	virtual int postStep();
};
}

#endif