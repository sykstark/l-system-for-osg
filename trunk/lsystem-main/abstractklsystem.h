#ifndef ABSTRACTKLSYSTEM_H_
#define ABSTRACTKLSYSTEM_H_

#include "lsystem.h"

namespace AP_LSystem {

/**
  * Abstract class for kL-Systems
  */
class AbstractkLSystem : public LSystem
{
protected:
	
public:
    /**
      * Constructor
      */
    AbstractkLSystem( AbstractFile * );
};
}
#endif // ABSTRACTKLSYSTEM_H_
