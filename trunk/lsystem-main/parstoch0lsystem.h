#ifndef PARSTOCH0LSYSTEM_H_
#define PARSTOCH0LSYSTEM_H_

#include "abstract0lsystem.h"

namespace AP_LSystem {
class ParStoch0LSystem : public Abstract0LSystem
{
private:
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC | LS_STOCHASTIC | LS_PARAMETRIC;
public:
    ParStoch0LSystem( AbstractFile * );

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

#endif // PARSTOCH0LSYSTEM_H_
