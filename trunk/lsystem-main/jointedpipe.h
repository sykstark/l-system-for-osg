#ifndef JOINTEDPIPE_H_
#define JOINTEDPIPE_H_

#include "loftturtle.h"

namespace AP_LSystem {
/**
  * Turtle that creates cylinders with hemispheres at the end of cylinders. This creates a geometry that is useful for
  * creating stems or some anorganic structures.
  */
class JointedPipe : public LoftTurtle
{
private:
	/**
	  * Create a hemisphere geometry.
	  */
	void createHemisphere();

	/**
	  * Append a hemisphere at the end of cylinder.
	  * @return error code
	  */
	virtual int addHemisphereGeometry( );
public:
	JointedPipe(void);
	~JointedPipe(void);

	/**
	  * Initialize turtle: create a contour and hemisphere template.
	  * @return error code
	  */
	virtual int initialize();

	/**
	  * Create a bottom contour of cylinder.
	  * @return error code
	  */
	virtual int preStep();

	/**
	  * Create a top contour of cylinder, create cylinder geometry and append hemisphere.
	  * @return error code
	  */
	virtual int postStep();

};
}

#endif