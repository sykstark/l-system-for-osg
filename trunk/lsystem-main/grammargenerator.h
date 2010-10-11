#ifndef GRAMMARGENERATOR_H_
#define GRAMMARGENERATOR_H_

#include "abstractgenerator.h"

namespace AP_LSystem {
class GrammarGenerator : public AbstractGenerator
{
public:
    GrammarGenerator(void);
    ~GrammarGenerator(void);

    virtual void loadFromFile( std::string & str );
    virtual void nextIteration();
};
}
#endif
