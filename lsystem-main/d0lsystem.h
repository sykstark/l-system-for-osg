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

    virtual bool transcribe(multimap<char, Rule> &);
public:
    D0LSystem( AbstractFile * );

    static bool isCapable( unsigned int type )
    {
        if( ( type & capabilities ) == type )
            return true;
        else
            return false;
    }
};
}

#endif // D0LSYSTEM_H_
