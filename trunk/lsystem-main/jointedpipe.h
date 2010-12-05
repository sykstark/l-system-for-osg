#ifndef JOINTEDPIPE_H_
#define JOINTEDPIPE_H_

#include "loftturtle.h"

namespace AP_LSystem {
class JointedPipe : public LoftTurtle
{
private:
	void createHemisphere();

	virtual int addHemisphereGeometry( );
public:
	JointedPipe(void);
	~JointedPipe(void);

	virtual int initialize();

	virtual int preStep();
	virtual int postStep();

};
}

#endif