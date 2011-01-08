#ifndef D0LSYSTEM_H_
#define D0LSYSTEM_H_

#include "abstract0lsystem.h"

namespace AP_LSystem {
class D0LSystem : public LSystem
{
protected:
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC;

    virtual void processPredecessor( Rule &, string *, string::iterator & );
    virtual void processRuleSuccessor( Rule &, string *, string::iterator &);
    virtual void processHomomorphismSuccessor( Rule &, string *, string::iterator &);

    /**
      * This method provides a transcription funkcionality. It substitutes all modules in word
      * by their successors.
      * @param rules container with rules
      * @return true if succeeded
      */
    virtual bool transcribe(multimap<char, Rule> & rules);
public:
    D0LSystem( AbstractFile * );

    /**
      * Decides if L-system of passed type can by processed by parametric
      * stochastic context-free L-system
      * @param type type of L-system that will be compared with D0LSytem capabilities
      * @return true if it is capable to process this type
      */
    static bool isCapable( unsigned int type )
    {
        if( ( type & capabilities ) == type )
            return true;
        else
            return false;
    }

    /**
      * Virtual copy constructor
      */
    shared_ptr<AbstractLSystem> clone() const
    {
        return shared_ptr<AbstractLSystem>( new D0LSystem(*this) );
    }
};
}

#endif // D0LSYSTEM_H_
