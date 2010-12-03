#ifndef LSYSTEMGENERATOR_H_
#define LSYSTEMGENERATOR_H_

#include "abstractgenerator.h"

namespace AP_LSystem {
/**
  * This is a default generator for L-system generating.
  */
class LSystemGenerator : public AbstractGenerator
{
public:
    /**
      * Non-parametric constructor
      */
    LSystemGenerator(void);

    /**
      * Destructor
      */
    ~LSystemGenerator(void);

    /**
      * Loads an L-system. File is specified by it's filename.
      * @param filename filename of file in one of supported L-system formats
      */
    virtual void loadFile( std::string & str );

    virtual void nextIteration();

    virtual ParseableString * getWord();
};
}
#endif
