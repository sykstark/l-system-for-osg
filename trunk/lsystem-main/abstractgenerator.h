#ifndef ABSTRACTGENERATOR_H_
#define ABSTRACTGENERATOR_H_

#include <string>
#include "parseablestring.h"
#include "abstractlsystem.h"

namespace AP_LSystem {
class AbstractGenerator
{
protected:
    ParseableString * m_Word;
    AbstractLSystem * m_MainLSystem;
	
public:
	AbstractGenerator(void)
	{
        m_Word = NULL;
        m_MainLSystem = NULL;
	}

	~AbstractGenerator(void)
	{
		// TODO
	}

    virtual void loadFile( std::string & ) = 0;
    virtual void nextIteration() = 0;
    virtual ParseableString * getWord() = 0;
	
    virtual void saveWordToFile( std::string & );
    virtual void loadWordFromFile( std::string & );

    static AbstractLSystem * createLSystem( AbstractFile * );
};
}

#endif
