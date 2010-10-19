//#pragma once

#ifndef LSFILEGRAMMAR_H_
#define LSFILEGRAMMAR_H_

#include <map>

#include "configuration.h"
#include "abstractgrammar.h"
#include "rule.h"

namespace AP_LSystem{
class LSFileGrammar : public AbstractGrammar
{
private:
	static const unsigned int capabilities = LS_0L | LS_DETERMINISTIC;
    LongString * _word;
    multimap<char, Rule> _rules;
    multimap<char, Rule> _homomorphisms;
    vector<string> _subGrammarsFilenames;
    map<string, string> _subGrammarsWords;

    bool getEndOfStatic(string *, string::iterator &, string::iterator &);

public:
    LSFileGrammar( AbstractFile * );
	~LSFileGrammar(void);

	virtual void addRule(std::string *);
    virtual void setAxiom(std::string &);
    virtual void addHomomorphism( std::string *);

    virtual void loadFromFile( AbstractFile * );

    virtual bool nextIteration();
    virtual LongString * translate( );

    unsigned int wordLength() { return _word->length(); }

	static bool isCapable( unsigned int type )
	{
		if( ( type & capabilities ) == type )
			return true;
		else
			return false;
	}
};
}

#endif
