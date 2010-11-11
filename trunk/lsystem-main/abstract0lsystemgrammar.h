#ifndef ABSTRACT0LSYSTEMGRAMMAR_H
#define ABSTRACT0LSYSTEMGRAMMAR_H

#include "lsystem.h"

namespace AP_LSystem {
class Abstract0LSystemGrammar : public LSystem
{
protected:
    virtual void addRule(std::string *);
    virtual void addHomomorphism( std::string *);
public:
    Abstract0LSystemGrammar();

};
}
#endif // ABSTRACT0LSYSTEMGRAMMAR_H
