#ifndef ABSTRACT0LSYSTEMGRAMMAR_H
#define ABSTRACT0LSYSTEMGRAMMAR_H

#include "lsystem.h"

namespace AP_LSystem {
class Abstract0LSystemGrammar : public LSystem
{
protected:
    virtual void processPredecessor( Rule &, string *, string::iterator & );
    virtual void processRuleSuccessor( Rule &, string *, string::iterator &);
    virtual void processHomomorphismSuccessor( Rule &, string *, string::iterator &);
public:
    Abstract0LSystemGrammar();

};
}
#endif // ABSTRACT0LSYSTEMGRAMMAR_H
