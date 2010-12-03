#ifndef ABSTRACT0LSYSTEM_H_
#define ABSTRACT0LSYSTEM_H_

#include "lsystem.h"

namespace AP_LSystem {
/**
  * Abstract class for context-free 0L-systems
  */
class Abstract0LSystem : public LSystem
{
protected:
    /**
      * Parses a predecessor of rule or homomorphism and stores it to Rule structure
      * @param rule structure of rule for storing parsed data
      * @param str string with rule
      * @param pos position of predecessor in rule string
      */
    virtual void processPredecessor( Rule & rule, string * str, string::iterator & pos);

    /**
      * Parses a rule successor and stores it to Rule structure
      * @param rule structure of rule for storing parsed data
      * @param str string with rule
      * @param pos position of predecessor in rule string
      */
    virtual void processRuleSuccessor( Rule & rule, string * str, string::iterator & pos);

    /**
      * Parses a homomorphism successor and stores it to Rule structure
      * @param rule structure of rule for storing parsed data
      * @param str string with rule
      * @param pos position of predecessor in rule string
      */
    virtual void processHomomorphismSuccessor( Rule & rule, string * str, string::iterator & pos);
public:
    Abstract0LSystem( AbstractFile * );
    Abstract0LSystem( const Abstract0LSystem & );

};
}
#endif // ABSTRACT0LSYSTEM_H_
