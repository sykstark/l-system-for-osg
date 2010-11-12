#ifndef PAR2LSYSTEMGRAMMAR_H
#define PAR2LSYSTEMGRAMMAR_H

#include "abstractklsystemgrammar.h"

namespace AP_LSystem {
class Par2LSystemGrammar : public AbstractkLSystemGrammar
{
private:
    static const unsigned int capabilities = LS_2L | LS_DETERMINISTIC | LS_PARAMETRIC;	
public:
    Par2LSystemGrammar( AbstractFile * );
    virtual void processPredecessor(Rule &, string *, string::iterator &);
    virtual void processRuleSuccessor(Rule &, string *, string::iterator &);
    virtual void processHomomorphismSuccessor(Rule &, string *, string::iterator &);
	virtual multimap<char, Rule>::iterator * selectRule(multimap<char, Rule>::iterator &, 
														multimap<char, Rule>::iterator &, 
														LongString *,
														unsigned int &,
														double *);

    static bool isCapable( unsigned int type )
    {
        if( ( type & capabilities ) == type )
            return true;
        else
            return false;
    }
};
}

#endif // PAR2LSYSTEMGRAMMAR_H
