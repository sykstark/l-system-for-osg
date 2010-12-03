#ifndef ABSTRACTLSYSTEM_H_
#define ABSTRACTLSYSTEM_H_

#include "fparser/fparser.hh"
#include "longstring.h"
#include "abstractfile.h"
#include "configuration.h"

using boost::shared_ptr;

namespace AP_LSystem {
class LongString;

/**
  * Abstract class for L-systems. It is basic interface for communication with
  * L-system generator.
  */
class AbstractLSystem
{
protected:
    std::string _name;
    unsigned int _iteration;

	virtual void addRule( std::string * ) = 0;
    virtual void addHomomorphism( std::string * ) = 0;
    virtual void setAxiom( std::string & ) = 0;
public:
    AbstractLSystem(): _name(""), _iteration(0) {}
    AbstractLSystem( const AbstractLSystem & c):_name(c._name), _iteration(c._iteration) {}

    /**
      * Virtual constructor
      */
    virtual shared_ptr<AbstractLSystem> clone() const = 0;

    /**
      * Assignment operator
      */
    AbstractLSystem & operator=(const AbstractLSystem & c )
    {
        _name = c._name;
        _iteration = c._iteration;
        return *this;
    }

    /**
      * Process next iteration. It uses transcription rules to get a new word.
      * @return true if succeeded
      */
    virtual bool nextIteration() = 0;

    /**
      * Translate a word to final form and get it.
      * @return word as LongString
      */
    virtual LongString * translate( ) = 0;
    
    /**
      * Load and initialize L-system according to it's parameters.
      * @param file Loaded L-system file
      */
    virtual void loadFromFile ( AbstractFile * file ) = 0;

    /**
      * Get current iteration.
      * @return current L-system iteration
      */
    unsigned int getIteration() const {return _iteration;}
};
}

#endif
