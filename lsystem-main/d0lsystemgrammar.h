#ifndef D0LSYSTEMGRAMMAR_H
#define D0LSYSTEMGRAMMAR_H

#include "abstract0lsystemgrammar.h"

namespace AP_LSystem {
class D0LSystemgrammar : public Abstract0LSystemGrammar
{
private:
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC;
public:
    D0LSystemgrammar();

    static bool isCapable( unsigned int type )
    {
        if( ( type & capabilities ) == type )
            return true;
        else
            return false;
    }
};
}

#endif // D0LSYSTEMGRAMMAR_H
