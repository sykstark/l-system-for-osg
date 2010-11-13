#ifndef LSYSTEMGENERATOR_H_
#define LSYSTEMGENERATOR_H_

#include "abstractgenerator.h"

namespace AP_LSystem {
class LSystemGenerator : public AbstractGenerator
{
public:
    LSystemGenerator(void);
    ~LSystemGenerator(void);

    virtual void loadFile( std::string & str );
    virtual void nextIteration();
    virtual ParseableString * getWord();
};
}
#endif
