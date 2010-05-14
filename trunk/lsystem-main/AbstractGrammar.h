#pragma once
#ifndef FPARSER
#define FPARSER
#include "fparser/fparser.hh"
#endif

#include <map>
#include "StringUtils.h"
#include "Rule.h"

using namespace std;

namespace AP_LSystem {
class LongString;
class AbstractGrammar
{
protected:
    LongString * _word;
    std::string _name;
    multimap<char, Rule> _rules;
    map<string, string> _homomorphisms;
    vector<string> _subGrammarsFilenames;
    int _iteration;
    std::string _axiom;
    map<string, string> _subGrammarsWords;	

public:
//	virtual ~LS_Grammar();
    AbstractGrammar():_word(NULL), _name(""), _iteration(0), _axiom("") {};
    bool generateSubGrammars(){ return false; };
    virtual void nextIteration() = 0;
    virtual char* translate() = 0;
    virtual void addRule( std::string * ) = 0;
    virtual void addHomomorphism( std::string * ) = 0;
    virtual void setAxiom( const std::string * ) = 0;
    virtual void loadFromFile ( std::string * ) = 0;
    int GetIteration() const {return _iteration;};
};
}
