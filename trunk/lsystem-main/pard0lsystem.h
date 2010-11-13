#ifndef PARD0LSYSTEM_H_
#define PARD0LSYSTEM_H_

#include "abstract0lsystem.h"

namespace AP_LSystem {
class ParD0LSystem : public Abstract0LSystem
{
private:
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC | LS_PARAMETRIC;
public:
    ParD0LSystem();

    static bool isCapable( unsigned int type )
    {
        if( ( type & capabilities ) == type )
            return true;
        else
            return false;
    }
};
}
#endif // PARD0LSYSTEM_H_
