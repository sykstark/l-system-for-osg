#ifndef ABSTRACTGENERATOR_H_
#define ABSTRACTGENERATOR_H_

#include <string>
#include "parseablestring.h"
#include "abstractlsystem.h"

namespace AP_LSystem {
class AbstractGenerator
{
protected:
    ParseableString * pWord;
    AbstractLSystem * pMainLSystem;
	
public:
	AbstractGenerator(void)
	{
		pWord = NULL;
		pMainLSystem = NULL;
	}

	~AbstractGenerator(void)
	{
		// TODO
	}

    virtual void loadFile( std::string & ) = 0;
    virtual void nextIteration() = 0;
    virtual ParseableString * getWord() = 0;
	
	virtual void saveWordToFile( std::string &)
	{
		// TODO
	}
	virtual void loadWordFromFile( std::string &)
	{
		// TODO
	}

    static AbstractLSystem * createLSystem( AbstractFile * );
};
}

#endif
