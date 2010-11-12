#ifndef ABSTRACTGRAMMAR_H_
#define ABSTRACTGRAMMAR_H_

#include "fparser/fparser.hh"
#include "longstring.h"
//#include "utils.h"
#include "abstractfile.h"
#include "configuration.h"

namespace AP_LSystem {
class LongString;

class AbstractGrammar
{
protected:
    std::string _name;
    unsigned int _iteration;

public:
    AbstractGrammar(): _name(""), _iteration(0) {}

    virtual bool nextIteration() = 0;
    virtual LongString * translate( ) = 0;
    virtual void addRule( std::string * ) = 0;
    virtual void addHomomorphism( std::string * ) = 0;
    virtual void setAxiom( std::string & ) = 0;
    virtual void loadFromFile ( AbstractFile * ) = 0;
    unsigned int getIteration() const {return _iteration;}

};
}

#endif
