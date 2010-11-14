#ifndef LSYSTEM_H_
#define LSYSTEM_H_

#include "abstractlsystem.h"
#include "rule.h"

namespace AP_LSystem {
class LSystem : public AbstractLSystem
{
protected:
    LongString * _word;
    multimap<char, Rule> _rules;
    multimap<char, Rule> _homomorphisms;
    vector<string> _subSystemsFilenames;
    vector< LongString *> _subSystemsWords;
	string ignore;

    // Loading and parsing
    virtual void setAxiom(std::string &);
    virtual void addRule(std::string *);
    virtual void addHomomorphism(std::string *);
    virtual void processPredecessor( Rule &, string *, string::iterator & ) = 0;
    virtual void processRuleSuccessor( Rule &, string *, string::iterator &) = 0;
    virtual void processHomomorphismSuccessor( Rule &, string *, string::iterator &) = 0;

    // Generating
    virtual bool nextIteration();
    virtual bool transcribe( multimap<char, Rule> & rules );
    virtual multimap<char, Rule>::iterator * selectRule(multimap<char, Rule>::iterator &,
                                                        multimap<char, Rule>::iterator &,
                                                        LongString *,
                                                        unsigned int &,
                                                        double *);
	virtual void generateSuccessor( LongString *, multimap<char, Rule>::iterator &, double *);
public:
    LSystem();

    virtual void loadFromFile( AbstractFile * );
    
    unsigned int wordLength() { return _word->length(); }
    virtual void transcribeSubSystems( );
    virtual LongString * translate( );
	virtual void processCutSymbol( );
};
}
#endif // LSYSTEM_H_
