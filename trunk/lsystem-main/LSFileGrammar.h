#pragma once
#include "AbstractGrammar.h"

namespace AP_LSystem{
class LSFileGrammar : public AbstractGrammar
{
private:
	

public:
	LSFileGrammar( std::string * );
	~LSFileGrammar(void);

	virtual void addRule(std::string *);
    virtual void setAxiom(const std::string &);
    virtual void addHomomorphism( std::string *);

	virtual void loadFromFile( std::string * );

	// temporarily
	virtual void nextIteration();
	virtual char* translate();

	unsigned int wordLength() { return _word->length(); };
};
}
