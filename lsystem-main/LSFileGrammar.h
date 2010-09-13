//#pragma once

#ifndef LSFILEGRAMMAR_H_
#define LSFILEGRAMMAR_H_

#include <map>

#include "AbstractGrammar.h"
#include "Rule.h"

namespace AP_LSystem{
class LSFileGrammar : public AbstractGrammar
{
private:
    LongString * _word;
    multimap<char, Rule> _rules;
    multimap<char, Rule> _homomorphisms;
    vector<string> _subGrammarsFilenames;
    map<string, string> _subGrammarsWords;

    bool getEndOfStatic(string *, string::iterator &, string::iterator &);

public:
	LSFileGrammar( std::string * );
	~LSFileGrammar(void);

	virtual void addRule(std::string *);
    virtual void setAxiom(std::string &);
    virtual void addHomomorphism( std::string *);

	virtual void loadFromFile( std::string * );

	// temporarily
    virtual bool nextIteration();
	virtual char* translate();

    unsigned int wordLength() { return _word->length(); }
};
}

#endif
