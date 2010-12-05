#ifndef TURTLEINTERPRET_H_
#define TURTLEINTERPRET_H_

#include "abstractinterpret.h"
#include "lsgeode.h"

namespace AP_LSystem {
class TurtleInterpret : public AbstractInterpret
{
private:
	
public:
	TurtleInterpret( osg::Group * owner );
	//TurtleInterpret( );
	~TurtleInterpret(void);

	virtual int parse( ParseableString * );
	osg::Group * setShadows( osg::Group * );
};
}

#endif