#ifndef LSYSTEMGRAMMAR_H
#define LSYSTEMGRAMMAR_H

//#include <map>
#include "abstractgrammar.h"
#include "rule.h"

namespace AP_LSystem {
class LSystemGrammar : public AbstractGrammar
{
protected:
    LongString * _word;
    multimap<char, Rule> _rules;
    multimap<char, Rule> _homomorphisms;
    vector<string> _subSystemsFilenames;
    vector< LongString *> _subSystemsWords;

    virtual void setAxiom(std::string &);
	virtual bool nextIteration();
	virtual multimap<char, Rule>::iterator * selectRule(multimap<char, Rule>::iterator &, 
														multimap<char, Rule>::iterator &, 
														LongString *,
														unsigned int &,
														double *);
	virtual void generateSuccessor( LongString *, multimap<char, Rule>::iterator &, double *);
public:
    LSystemGrammar();

    virtual void loadFromFile( AbstractFile * );
    
    unsigned int wordLength() { return _word->length(); }
    virtual void transcribeSubSystems( );
    virtual LongString * translate( );
	virtual void processCutSymbol( );
};
}
#endif // LSYSTEMGRAMMAR_H
