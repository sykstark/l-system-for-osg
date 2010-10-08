//#pragma once

#ifndef ABSTRACTGRAMMAR_H_
#define ABSTRACTGRAMMAR_H_

#include "fparser/fparser.hh"
#include "StringUtils.h"
#include "abstractfile.h"

namespace AP_LSystem {
class AbstractGrammar
{
protected:
    std::string _name;
    int _iteration;

public:
    AbstractGrammar(): _name(""), _iteration(0) {}
    bool generateSubGrammars(){ return false; }
    virtual bool nextIteration() = 0;
    virtual char* translate() = 0;
    virtual void addRule( std::string * ) = 0;
    virtual void addHomomorphism( std::string * ) = 0;
    virtual void setAxiom( std::string & ) = 0;
    virtual void loadFromFile ( AbstractFile * ) = 0;
    int GetIteration() const {return _iteration;}
};
}

#endif
