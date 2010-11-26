#ifndef ABSTRACT0LSYSTEM_H_
#define ABSTRACT0LSYSTEM_H_

#include "lsystem.h"

namespace AP_LSystem {
class Abstract0LSystem : public LSystem
{
protected:
    virtual void processPredecessor( Rule &, string *, string::iterator & );
    virtual void processRuleSuccessor( Rule &, string *, string::iterator &);
    virtual void processHomomorphismSuccessor( Rule &, string *, string::iterator &);
public:
    Abstract0LSystem( AbstractFile * );

};
}
#endif // ABSTRACT0LSYSTEM_H_
