#ifndef PARSTOCH0LSYSTEMGRAMMAR_H
#define PARSTOCH0LSYSTEMGRAMMAR_H

#include "abstract0lsystemgrammar.h"

namespace AP_LSystem {
class ParStoch0LSystemGrammar : public Abstract0LSystemGrammar
{
private:
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC | LS_STOCHASTIC | LS_PARAMETRIC;
public:
    ParStoch0LSystemGrammar( AbstractFile * );

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

#endif // PARSTOCH0LSYSTEMGRAMMAR_H
