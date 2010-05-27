#pragma once
#ifndef FPARSER
#define FPARSER
#include "fparser/fparser.hh"
#endif

#include "StringUtils.h"

using namespace std;

namespace AP_LSystem {
class AbstractGrammar
{
protected:
    std::string _name;
    int _iteration;

public:
    AbstractGrammar(): _name(""), _iteration(0) {};
    bool generateSubGrammars(){ return false; };
    virtual bool nextIteration() = 0;
    virtual char* translate() = 0;
    virtual void addRule( std::string * ) = 0;
    virtual void addHomomorphism( std::string * ) = 0;
    virtual void setAxiom( const std::string & ) = 0;
    virtual void loadFromFile ( std::string * ) = 0;
    int GetIteration() const {return _iteration;};
};
}
