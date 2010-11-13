#ifndef D0LSYSTEM_H_
#define D0LSYSTEM_H_

#include "abstract0lsystem.h"

namespace AP_LSystem {
class D0LSystem : public Abstract0LSystem
{
private:
    static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC;
public:
    D0LSystem();

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
