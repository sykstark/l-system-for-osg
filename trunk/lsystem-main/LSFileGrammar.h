#pragma once

#include <map>

#include "AbstractGrammar.h"
#include "Rule.h"

namespace AP_LSystem{
class LSFileGrammar : public AbstractGrammar
{
private:
    LongString * _word;
    multimap<char, Rule> _rules;
    map<string, string> _homomorphisms;
    vector<string> _subGrammarsFilenames;
    map<string, string> _subGrammarsWords;
    //std::string _axiom;

public:
	LSFileGrammar( std::string * );
	~LSFileGrammar(void);

	virtual void addRule(std::string *);
    virtual void setAxiom(const std::string &);
    virtual void addHomomorphism( std::string *);

	virtual void loadFromFile( std::string * );

	// temporarily
    virtual bool nextIteration();
	virtual char* translate();

	unsigned int wordLength() { return _word->length(); };
};
}
