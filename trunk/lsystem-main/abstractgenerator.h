#ifndef ABSTRACTGENERATOR_H_
#define ABSTRACTGENERATOR_H_

#include <string>
#include "longstring.h"
#include "AbstractGrammar.h"

namespace AP_LSystem {
class AbstractGenerator
{
protected:

    LongString * pWord;
    AbstractGrammar * pMainGrammar;
	
public:
	AbstractGenerator(void)
	{

	}

	~AbstractGenerator(void)
	{
	}

    virtual void loadFromFile( std::string &) = 0;
    virtual void nextIteration() = 0;
};
}

#endif
