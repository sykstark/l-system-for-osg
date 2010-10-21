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
    vector<string> _subGrammarsFilenames;
    map<string, string> _subGrammarsWords;

    virtual void setAxiom(std::string &);
public:
    LSystemGrammar();

    virtual void loadFromFile( AbstractFile * );
    virtual void generateSuccessor( LongString *, multimap<char, Rule>::iterator &, double *);
    virtual multimap<char, Rule>::iterator & selectRule(multimap<char, Rule>::iterator &, multimap<char, Rule>::iterator &);
    unsigned int wordLength() { return _word->length(); }
    virtual LongString * translate( );
};
}
#endif // LSYSTEMGRAMMAR_H
