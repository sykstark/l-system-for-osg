#ifndef PARD0LSYSTEMGRAMMAR_H
#define PARD0LSYSTEMGRAMMAR_H

#include "abstract0lsystemgrammar.h"

namespace AP_LSystem {
class ParD0LSystemGrammar : public Abstract0LSystemGrammar
{
private:
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC | LS_PARAMETRIC;
public:
    ParD0LSystemGrammar();

    static bool isCapable( unsigned int type )
    {
        if( ( type & capabilities ) == type )
            return true;
        else
            return false;
    }
};
}
#endif // PARD0LSYSTEMGRAMMAR_H
